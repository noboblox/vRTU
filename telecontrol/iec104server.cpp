/*
 * iec104server.cpp
 *
 *  Created on: 16.12.2019
 *      Author: rene
 */

#include "iec104server.hpp"

#include <mutex>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <string>

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
  public:
    using DataMap = std::unordered_map<std::string, SpDataPtr>;

    static constexpr size_t msDefaultQueueSize = 1024;

    Iec104ServerImpl(const Iec104ServerParameter& arSettings)
      : mpSlave(CS104_Slave_create(msDefaultQueueSize, msDefaultQueueSize)),
        mId(arSettings.mId)
    {

      if (!CheckPreconditions(arSettings))
        throw std::invalid_argument("Unsupported arguments provided");

      const std::string ipString = arSettings.mIpAddress.to_string();
      CS104_Slave_setLocalAddress(mpSlave, ipString.c_str());
      CS104_Slave_setLocalPort(mpSlave, arSettings.mPort);
      SetupHandlers();
    }

    virtual ~Iec104ServerImpl()
    {
      CS104_Slave_destroy(mpSlave);
    }

    void StopServer()
    {
      const std::lock_guard<std::mutex> owner(mAccess);
      CS104_Slave_stop(mpSlave);
    }

    void StartServer()
    {
      const std::lock_guard<std::mutex> owner(mAccess);
      CS104_Slave_start(mpSlave);
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

    void UnregisterData(const std::string& arId)
    {
      const std::lock_guard<std::mutex> owner(mAccess);

      DataMap::iterator target;
      if ((target = mStatusData.find(arId)) != mStatusData.end())
        mStatusData.erase(target);
      if ((target = mControlData.find(arId)) != mControlData.end())
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

        DataMap::iterator target = mStatusData.find(arId);

        if (target == mStatusData.end())
          throw std::invalid_argument("Cannot update. Data does not exist.");

        pTarget = target->second;
      } // unlock
      
      pTarget->Update(arUpdate);
    }

  private: // Private functions without own locks

    void AssertUniqueId(const std::string& arId) const
    {
      if ((mStatusData.find(arId) != mStatusData.cend()) ||
        (mControlData.find(arId) != mControlData.cend()))
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

  private: // Private functions with own locks
         
    void RegisterInternal(const TC::BasePropertyList& arDefinition, DataMap& arDestination)
    {
      const CS101_AppLayerParameters pParam = CS104_Slave_getAppLayerParameters(mpSlave);
      SpDataPtr spNewData = IEC104::DataFactory::Create(arDefinition, pParam->sizeOfIOA);

      if (!spNewData)
        throw std::runtime_error("Failed to create new data point.");

      const std::string dataId = spNewData->GetId();

      {
        const std::lock_guard<std::mutex> owner(mAccess);
        AssertUniqueId(dataId);
        arDestination.insert(std::make_pair(dataId, std::move(spNewData)));
      }
    }

    bool OnConnectRequest(const std::string& arIpAdress) noexcept
    {
      return true;
    }

    void OnConnectEvent(IMasterConnection apConnection, CS104_PeerConnectionEvent aEvent) noexcept
    {
      const std::lock_guard<std::mutex> owner(mAccess);

      switch(aEvent)
      {
        case CS104_CON_EVENT_CONNECTION_OPENED:
        case CS104_CON_EVENT_CONNECTION_CLOSED:
        case CS104_CON_EVENT_ACTIVATED:
        case CS104_CON_EVENT_DEACTIVATED:
        default:
          break;
      }

      // TODO
    }

    bool OnInterrogation(IMasterConnection apConnection, CS101_ASDU apAsdu, uint8_t aQoi) noexcept
    {
      const std::lock_guard<std::mutex> owner(mAccess);

      if (!ProcessServerId(apConnection, apAsdu))
        return true;

      return true; // TODO
    }

    bool OnCounterInterrogation(IMasterConnection apConnection, CS101_ASDU apAsdu, QualifierOfCIC aQcc) noexcept
    {
      const std::lock_guard<std::mutex> owner(mAccess);

      if (!ProcessServerId(apConnection, apAsdu))
        return true;

      return true; // TODO
    }

    bool OnDefaultAsduEvent(IMasterConnection apConnection, CS101_ASDU apAsdu) noexcept
    {
      const std::lock_guard<std::mutex> owner(mAccess);

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
    DataMap mStatusData;
    DataMap mControlData;

    mutable std::mutex mAccess;
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
