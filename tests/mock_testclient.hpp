/*
 * mock_testclient.hpp
 *
 *  Created on: 18.12.2019
 *      Author: rene
 */

#ifdef MOCK_TESTCLIENT_HPP_
#error "This header contains the global test client mock. Do not use multiple times."
#else
#define MOCK_TESTCLIENT_HPP_

#include "iec104server.hpp"

#include <memory>
#include <chrono>
#include <thread>
#include <string>
#include <map>

extern "C"
{
#include "cs104_connection.h"
#include <cstring>
}

class IecTestClient
{
public:

  IecTestClient()
    : mpMaster(CS104_Connection_create("127.0.0.1", IEC104::Iec104ServerParameter::msDefaultServerPort))
  {
    CS104_Connection_setASDUReceivedHandler(mpMaster, &IecTestClient::sGlobalAsduReceive, this);
  }

  explicit IecTestClient(uint16_t aPort)
    : mpMaster(CS104_Connection_create("127.0.0.1", aPort)) {}

  bool Connect()
  {
    return CS104_Connection_connect(mpMaster);
  }

  void StartMonitoring()
  {
    CS104_Connection_sendStartDT(mpMaster);
  }


  static bool sGlobalAsduReceive(void* apSelf, int aUnused, CS101_ASDU apAsdu)
  {
    return true;
  }

  ~IecTestClient()
  {
    CS104_Connection_destroy(mpMaster);
  }
private:
  CS104_Connection mpMaster;
};


static std::unique_ptr<IecTestClient> gpTestClient = nullptr;

static void InitClient()
{
  gpTestClient.reset(new IecTestClient());
}

static void KillClient()
{
  gpTestClient.reset(nullptr);
}




#endif

