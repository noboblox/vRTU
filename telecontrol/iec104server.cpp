/*
 * iec104server.cpp
 *
 *  Created on: 16.12.2019
 *      Author: rene
 */

#include "iec104server.hpp"

#include <mutex>
#include <thread>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

#include "iec104data.hpp"

extern "C"
{
#include <cassert>
#include "cs104_slave.h"
}

namespace IEC104
{
  class Iec104ServerImpl
  {
  public: // public functions need to be always thread safe
    static constexpr size_t msDefaultQueueSize = 1024;

    using DataBuffer = std::vector<SpBaseInformation>;

    class DataSettings
    {
    public:
      DataSettings(const std::string& arId, bool aDoCycle, bool aDoGI)
        : mInfoObjectId(arId),
          mSendOnCycle(aDoCycle),
          mSendOnInterrogation(aDoGI) {}

      explicit DataSettings(const std::string& arId)
        : mInfoObjectId(arId),
          mSendOnCycle(true),
          mSendOnInterrogation(true) {}

      const bool IsCycleData() const {return mSendOnCycle;}
      const bool IsInterrogationData() const {return mSendOnInterrogation;}
      const std::string& GetId() const {return mInfoObjectId;}

      const bool operator< (const DataSettings& arOther) const {return mInfoObjectId <  arOther.mInfoObjectId;}
      const bool operator==(const DataSettings& arOther) const {return mInfoObjectId == arOther.mInfoObjectId;}

    private:
      std::string mInfoObjectId;
      bool mSendOnCycle;
      bool mSendOnInterrogation;
    };

    class ConnectionStatus
    {
    public:
      explicit ConnectionStatus(const std::string& arRemoteHost)
        : mHostString(arRemoteHost), mActive(false) {}

      const std::string& GetRemoteHost() const {return mHostString;}
      bool IsActive() const {return mActive;}

      void Activate() {mActive = true;}
      void Deactivate() {mActive = false;}

    private:
      std::string mHostString;
      bool mActive;

    };

    using ConnectionMap = std::map<IMasterConnection, ConnectionStatus>;
    using DataMap = std::vector<std::pair<DataSettings, SpDataPtr>>;

    Iec104ServerImpl(const Iec104ServerParameter& arSettings)
      : mpSlave(nullptr),
        mId(arSettings.mId), mSendCycle(arSettings.mCycle),
        mIpAddress(arSettings.mIpAddress), mPort(arSettings.mPort),
        mSignalStop(false)
    {

      if (!CheckPreconditions(arSettings))
        throw std::invalid_argument("Unsupported arguments provided");
      InitCleanSlaveObject(mIpAddress, mPort);
    }

    void InitCleanSlaveObject(const boost::asio::ip::address& arIp, uint16_t aPort)
    {
      if (mpSlave)
      {
        CS104_Slave_destroy(mpSlave);
        mpSlave = nullptr;
      }

      const std::string ip_string = arIp.to_string();
      mpSlave = CS104_Slave_create(msDefaultQueueSize, msDefaultQueueSize);
      CS104_Slave_setLocalAddress(mpSlave, ip_string.c_str());
      CS104_Slave_setLocalPort(mpSlave, aPort);
      SetupHandlers();
    }

    virtual ~Iec104ServerImpl()
    {
      StopServer();
      if (mpSlave)
        CS104_Slave_destroy(mpSlave);
    }

    void StopServer()
    {
      // requests child threads to finish
      {
        const std::lock_guard<std::mutex> owner(mAccess);
        mSignalStop = true;
      }

      // wait for childs to finish
      for (auto& thread : mChildThreads)
      {
        if (thread.joinable())
          thread.join();
      }
      mChildThreads.clear();

      // revert private data to initial state
      {
        const std::lock_guard<std::mutex> owner(mAccess);
        mSignalStop = false;
        InitCleanSlaveObject(mIpAddress, mPort);
      }
    }

    void StartServer()
    {
      const std::lock_guard<std::mutex> owner(mAccess);

      if (CS104_Slave_isRunning(mpSlave))
        return;

      CS104_Slave_startThreadless(mpSlave);

      if (!CS104_Slave_isRunning(mpSlave))
        throw std::runtime_error("Unable to start server on requested local interface (Socket in use?).");

      const std::chrono::milliseconds RECEIVE_CYCLE = std::chrono::milliseconds(10);
      mChildThreads.emplace_back(std::thread(&Iec104ServerImpl::ServerLoop, this, RECEIVE_CYCLE, &Iec104ServerImpl::HandleIncomingRequests));

      const std::chrono::milliseconds SEND_CYCLE = (mSendCycle != std::chrono::milliseconds(0)) ? mSendCycle : std::chrono::milliseconds(2000);
      mChildThreads.emplace_back(std::thread(&Iec104ServerImpl::ServerLoop, this, SEND_CYCLE, &Iec104ServerImpl::DoSendCycle));
    }

    bool IsStopRequested() const
    {
      const std::lock_guard<std::mutex> owner(mAccess);
      return mSignalStop;
    }

    void ServerLoop(const std::chrono::microseconds aCycle, std::function<void(Iec104ServerImpl&)> arCallee)
    {
      while (!IsStopRequested())
      {
        try
        {
          arCallee(*this);
        }
        catch (std::exception& e)
        {
          std::cerr << "Critical error in server loop (Server = " << mId << ") - " << e.what() << std::endl;
        }
        catch (...)
        {
          std::cerr << "Critical error. Unkown exception in server loop" << mId << std::endl;
        }
        std::this_thread::sleep_for(aCycle);
      }
    }


    void
    RegisterStatusData(const TC::BasePropertyList& arDefinition)
    {
      RegisterInternal(arDefinition, mStatusData);
    }
    
    void
    RegisterControlData(const TC::BasePropertyList& arDefinition)
    {
      RegisterInternal(arDefinition, mControlData);
    }

    void
    UnregisterData(const std::string& arId)
    {
      const std::lock_guard<std::mutex> owner(mAccess);

      DataMap::iterator target;
      if ((target = FindData(mStatusData, DataSettings(arId))) != mStatusData.end())
        mStatusData.erase(target);
      if ((target = FindData(mControlData, DataSettings(arId))) != mControlData.end())
        mControlData.erase(target);
    }

    size_t CountStatusData() const
    {
      const std::lock_guard<std::mutex> owner(mAccess);
      return mStatusData.size();
    }

    size_t CountControlData() const
    {
      const std::lock_guard<std::mutex> owner(mAccess);
      return mControlData.size();
    }

    void UpdateStatusData(const std::string& arId, const TC::BaseDataPropertyList& arUpdate)
    {
      SpDataPtr pTarget = nullptr;

      {
        const std::lock_guard<std::mutex> owner(mAccess);

        DataMap::iterator target = FindData(mStatusData, DataSettings(arId));

        if (target == mStatusData.end())
          throw std::invalid_argument("Cannot update. Data does not exist.");

        pTarget = target->second;
      } // unlock
      
      pTarget->Update(arUpdate);
    }

  private: // Private functions without own locks

    /**
     * @brief Sorts all IOs and sends an ASDU for each TypeId.
     *
     * @note arSendBuffer is cleared afterwards.
     */
    void SendAll(std::vector<SpBaseInformation>& arSendBuffer) const
    {
      auto CompareFunc = [] (const SpBaseInformation& arLeft, const SpBaseInformation& arRight) -> bool
      {
        return InformationObject_getType(arLeft.Get()) < InformationObject_getType(arRight.Get());
      };

      std::sort(arSendBuffer.begin(), arSendBuffer.end(), CompareFunc);

      auto it = arSendBuffer.cbegin();
      auto it_end = arSendBuffer.cend();

      while (it != it_end)
      {
        const auto sameTypeRange = std::equal_range(it, it_end, *it, CompareFunc);
        SendSingleType(sameTypeRange.first, sameTypeRange.second);
        it = sameTypeRange.second;
      }
      arSendBuffer.clear();
    }

    void SendSingleType(const DataBuffer::const_iterator& arBegin, const DataBuffer::const_iterator& arEnd) const
    {
      if (arBegin == arEnd)
        return;

      static constexpr bool isSequence = false;              // TODO VS = 1
      static constexpr bool isNegative = false;
      static constexpr bool isTest = false;
      static constexpr CS101_CauseOfTransmission reasonCode = CS101_COT_PERIODIC; // TODO call this function for non cycle data
      static constexpr int originId = 0;                    // TODO support origin address

      int commonAddress = 0;
      sCS101_AppLayerParameters serverParameters{};
      {
        const std::lock_guard<std::mutex> owner(mAccess);

        commonAddress = mId;
        CS101_AppLayerParameters pParams = CS104_Slave_getAppLayerParameters(mpSlave);
        if (pParams)
          serverParameters = *pParams;
      }

      sCS101_StaticASDU asdu{};
      CS101_ASDU_initializeStatic(&asdu, &serverParameters, isSequence, reasonCode, originId, commonAddress, isTest, isNegative);

      DataBuffer::const_iterator it = arBegin;
      CS101_ASDU pAsdu = reinterpret_cast<CS101_ASDU>(&asdu);

      // TODO asdu > buffer size ! -> send and create new asdu
      for (; it != arEnd; ++it)
      {
        CS101_ASDU_addInformationObject(pAsdu, it->Get());
      }
      CS104_Slave_enqueueASDU(mpSlave, pAsdu);
    }


    DataMap::const_iterator FindData(const DataMap& arSearched, const DataSettings& arId) const
    {
      DataMap::const_iterator it = arSearched.cbegin();
      for (; it != arSearched.cend(); ++it)
      {
        if (it->first == arId)
          return it;
      }
      return arSearched.cend();
    }

    DataMap::iterator FindData(DataMap& arSearched, const DataSettings& arId)
    {
      DataMap::iterator it = arSearched.begin();
      for (; it != arSearched.end(); ++it)
      {
        if (it->first == arId)
          return it;
      }
      return arSearched.end();
    }

    void AssertUniqueId(const DataSettings& arId) const
    {
      if ((FindData(mStatusData, arId) != mStatusData.cend()) ||
          (FindData(mControlData, arId) != mControlData.cend()))
      {
        throw std::invalid_argument("Cannot handle registration request. Data with the same id is registered already.");
      }
    }

    bool CheckPreconditions(const Iec104ServerParameter& arSettings)
    {
      if (!arSettings.mIpAddress.is_v4() && !arSettings.mIpAddress.is_v6())
        return false;
      if (arSettings.mId == 0)
        return false;

      return true;
    }

    bool IsRequestForOwnId(CS101_ASDU apRequest) const
    {
      const uint16_t requestedServerId = static_cast<uint16_t> (CS101_ASDU_getCA(apRequest));
      return (mId == requestedServerId);
    }

    bool ProcessServerId(IMasterConnection apConnection, CS101_ASDU apAsdu)
    {
      if (!IsRequestForOwnId(apAsdu))
      {
        CS101_ASDU_setCOT(apAsdu, CS101_COT_UNKNOWN_CA);
        CS101_ASDU_setNegative(apAsdu, true);
        IMasterConnection_sendASDU(apConnection, apAsdu);
        return false;
      }
      return true;
    }

  private: // Private functions that do lock

    void HandleIncomingRequests()
    {
      const std::lock_guard<std::mutex> owner(mAccess);
      CS104_Slave_tick(mpSlave);
    }

    void DoSendCycle() const
    {

      static std::vector<SpBaseInformation> sendBuffer;
      sendBuffer.clear();

      if (GetActiveConnections() == 0) // Periodic data is sent after STARTDT only
        return;

      { // Critical section
        const std::lock_guard<std::mutex> owner(mAccess);


        for (const auto& entry : mStatusData)
        {
          if (entry.first.IsCycleData())
            sendBuffer.push_back(entry.second->Write());
        }
      }

      sendBuffer.shrink_to_fit();
      SendAll(sendBuffer);
    }
         
    void RegisterInternal(const TC::BasePropertyList& arDefinition, DataMap& arDestination)
    {
      const CS101_AppLayerParameters pParam = CS104_Slave_getAppLayerParameters(mpSlave);
      DataFactory factory(pParam->sizeOfIOA);
      SpDataPtr spNewData = factory.Create(arDefinition);

      if (!spNewData)
        throw std::runtime_error("Failed to create new data point.");

      const DataSettings dataId(spNewData->GetId());

      {
        const std::lock_guard<std::mutex> owner(mAccess);
        AssertUniqueId(dataId);
        arDestination.push_back(std::make_pair(dataId, std::move(spNewData)));
      }
    }

    bool OnConnectRequest(const std::string& arIpAdress) noexcept
    {
      return true;
    }

    void OnConnectEvent(IMasterConnection apConnection, CS104_PeerConnectionEvent aEvent) noexcept
    {
      static constexpr size_t bufferSize = 64;
      char ipString[bufferSize + 1] = "";
      IMasterConnection_getPeerAddress(apConnection, ipString, bufferSize);

      const std::lock_guard<std::mutex> owner(mAccess);

      if (aEvent == CS104_CON_EVENT_CONNECTION_OPENED)
      {
        mConnections.insert(std::make_pair(apConnection, ConnectionStatus(std::string(ipString))));
        return;
      }

      ConnectionMap::iterator itTarget = mConnections.find(apConnection);
      if (itTarget == mConnections.end())
        return;

      switch(aEvent)
      {
        case CS104_CON_EVENT_CONNECTION_CLOSED:
          mConnections.erase(itTarget);
          break;
        case CS104_CON_EVENT_ACTIVATED:
          itTarget->second.Activate();
          break;
        case CS104_CON_EVENT_DEACTIVATED:
          itTarget->second.Deactivate();
          break;

        default:
          break;
      }
    }

    size_t GetActiveConnections() const
    {
      const std::lock_guard<std::mutex> owner(mAccess);
      size_t result = 0;
      for(const auto& connection : mConnections)
      {
        if (connection.second.IsActive())
          ++result;
      }
      return result;
    }

    size_t GetOpenConnections() const
    {
      const std::lock_guard<std::mutex> owner(mAccess);
      return mConnections.size();
    }

    bool OnInterrogation(IMasterConnection apConnection, CS101_ASDU apAsdu, uint8_t aQoi) noexcept
    {
      if (!ProcessServerId(apConnection, apAsdu))
        return true;

      return true; // TODO
    }

    bool OnCounterInterrogation(IMasterConnection apConnection, CS101_ASDU apAsdu, QualifierOfCIC aQcc) noexcept
    {
      if (!ProcessServerId(apConnection, apAsdu))
        return true;

      return true; // TODO
    }

    bool OnDefaultAsduEvent(IMasterConnection apConnection, CS101_ASDU apAsdu) noexcept
    {
      if (!ProcessServerId(apConnection, apAsdu))
        return true;

      return true; // TODO
    }

    void SetupHandlers()
    {
      CS104_Slave_setConnectionRequestHandler(mpSlave, &Iec104ServerImpl::sConnectionRequestHandler, this);
      CS104_Slave_setConnectionEventHandler(mpSlave, &Iec104ServerImpl::sConnectionEventHandler, this);
      CS104_Slave_setInterrogationHandler(mpSlave, &Iec104ServerImpl::sInterrogation, this);
      CS104_Slave_setCounterInterrogationHandler(mpSlave, &Iec104ServerImpl::sCounterInterrogation, this);
      CS104_Slave_setASDUHandler(mpSlave, &Iec104ServerImpl::sAsduHandler, this);
    }

  private: // Global handlers for events from stack
    static bool sConnectionRequestHandler(void* apSelf, const char* strIpAddress)
    {
      assert(apSelf != nullptr);
      return static_cast<Iec104ServerImpl*> (apSelf)->OnConnectRequest(strIpAddress);
    }
    static void sConnectionEventHandler(void* apSelf, IMasterConnection apConnection, CS104_PeerConnectionEvent aEvent)
    {
      assert(apSelf != nullptr);
      static_cast<Iec104ServerImpl*> (apSelf)->OnConnectEvent(apConnection, aEvent);
    }
    static bool sInterrogation(void* apSelf, IMasterConnection apConnection, CS101_ASDU apAsdu, uint8_t aQoi)
    {
      assert(apSelf != nullptr);
      return static_cast<Iec104ServerImpl*> (apSelf)->OnInterrogation(apConnection, apAsdu, aQoi);
    }
    static bool sCounterInterrogation(void* apSelf, IMasterConnection apConnection, CS101_ASDU apAsdu, QualifierOfCIC aQcc)
    {
      assert(apSelf != nullptr);
      return static_cast<Iec104ServerImpl*> (apSelf)->OnCounterInterrogation(apConnection, apAsdu, aQcc);
    }
    static bool sAsduHandler(void* apSelf, IMasterConnection apConnection, CS101_ASDU apAsdu)
    {
      assert(apSelf != nullptr);
      return static_cast<Iec104ServerImpl*> (apSelf)->OnDefaultAsduEvent(apConnection, apAsdu);
    }

  private:
    CS104_Slave mpSlave;
    uint16_t mId;
    std::chrono::milliseconds mSendCycle;
    boost::asio::ip::address mIpAddress;
    uint16_t mPort;
    bool mSignalStop;

    DataMap mStatusData;
    DataMap mControlData;
    ConnectionMap mConnections;
    std::vector<std::thread> mChildThreads;

    mutable std::mutex mAccess; //!< Thread safe access to member data
  };


  Iec104Server::Iec104Server(const Iec104ServerParameter& arSettings)
    : TelecontrolServerInterface(),
      mpImpl(new Iec104ServerImpl(arSettings))
  {
  }

  Iec104Server::~Iec104Server()
  {
    delete mpImpl;
  }

  void
  Iec104Server::StartServer()
  {
    mpImpl->StartServer();
  }

  void
  Iec104Server::StopServer()
  {
    mpImpl->StopServer();
  }

  void 
  Iec104Server::RegisterControlData(const TC::BasePropertyList& arDefinition)
  {
    mpImpl->RegisterControlData(arDefinition);
  }

  void
  Iec104Server::RegisterStatusData(const TC::BasePropertyList& arDefinition)
  {
    mpImpl->RegisterStatusData(arDefinition);
  }

  void
  Iec104Server::UnregisterData(const std::string& arId)
  {
    mpImpl->UnregisterData(arId);
  }

  size_t
  Iec104Server::CountStatusData() const
  {
    return mpImpl->CountStatusData();
  }

  size_t
  Iec104Server::CountControlData() const
  {
    return mpImpl->CountControlData();
  }

  void
  Iec104Server::UpdateStatusData(const std::string& arId, const TC::BaseDataPropertyList& arUpdate)
  {
    mpImpl->UpdateStatusData(arId, arUpdate);
  }



} /* namespace TC */
