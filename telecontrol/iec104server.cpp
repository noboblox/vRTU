/*
 * iec104server.cpp
 *
 *  Created on: 16.12.2019
 *      Author: rene
 */

#include "iec104server.hpp"
#include <mutex>

extern "C"
{
#include <cassert>
#include "cs104_slave.h"
}

namespace TC
{

  class Iec104ServerImpl
  {
  public:
    static constexpr size_t msDefaultQueueSize = 1024;

    Iec104ServerImpl(const Iec104ServerParameter& arSettings)
      : mpSlave(CS104_Slave_create(msDefaultQueueSize, msDefaultQueueSize)),
        mId(arSettings.mId)
    {

      if (!CheckPreconditions(arSettings))
        throw std::invalid_argument("Unsupported arguments provided");

      const char* ipString = arSettings.mIpAddress.to_string().c_str();
      CS104_Slave_setLocalAddress(mpSlave, ipString);
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

  private: // Private functions without own locks
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

  private: // Private functions with own locks
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



} /* namespace TC */
