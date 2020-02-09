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
#include "iec104data.hpp"

#include <memory>
#include <mutex>
#include <chrono>
#include <thread>
#include <string>
#include <map>
#include <array>
#include <vector>

extern "C"
{
#include "cs104_connection.h"
#include <cstring>
}

class IecTestClient
{
public:

  IecTestClient()
    : mpMaster(CS104_Connection_create("127.0.0.1", IEC104::Iec104ServerParameter::msDefaultServerPort)),
      mInternalBuffer(),
      mPositionNextWrite(0),
      mPositionNextRead(0),
      mUnique()
  {
    CS104_Connection_setASDUReceivedHandler(mpMaster, &IecTestClient::sGlobalAsduReceive, this);
  }

  explicit IecTestClient(uint16_t aPort)
    : mpMaster(CS104_Connection_create("127.0.0.1", aPort)),
      mInternalBuffer(),
      mPositionNextWrite(0),
      mPositionNextRead(0),
      mUnique() {}

  bool Connect()
  {
    return CS104_Connection_connect(mpMaster);
  }

  void StartMonitoring()
  {
    CS104_Connection_sendStartDT(mpMaster);
  }

  void StopMonitoring()
  {
    CS104_Connection_sendStopDT(mpMaster);
  }

  static bool sGlobalAsduReceive(void* apSelf, int aUnused, CS101_ASDU apAsdu)
  {
    if (!apSelf || ! apAsdu)
      return false;
    return static_cast<IecTestClient*>(apSelf)->ProcessAsdu(apAsdu);
  }

  bool ProcessAsdu(CS101_ASDU apAsdu)
  {
    const size_t count = CS101_ASDU_getNumberOfElements(apAsdu);
    const size_t commonAddress = CS101_ASDU_getCA(apAsdu);

    int i = 0;
    for (; i < count; ++i)
    {
      std::lock_guard<std::mutex> lock(mUnique);
      mInternalBuffer[mPositionNextWrite] = IEC104::SpBaseInformation(CS101_ASDU_getElement(apAsdu, i));
      ++mPositionNextWrite;
    }
    return true;
  }

  void ReadBuffer(std::vector<IEC104::SpBaseInformation>& arResult) const
  {
    std::lock_guard<std::mutex> lock(mUnique);
    while (mInternalBuffer[mPositionNextRead].Get() != nullptr)
    {
      IEC104::SpBaseInformation& rData = mInternalBuffer[mPositionNextRead];
      arResult.push_back(std::move(rData));
      ++mPositionNextRead;
    }
  }
  ~IecTestClient()
  {
    CS104_Connection_destroy(mpMaster);
  }
private:
  CS104_Connection mpMaster;
  mutable std::array<IEC104::SpBaseInformation, 256> mInternalBuffer;
  uint8_t mPositionNextWrite;
  mutable uint8_t mPositionNextRead;

  mutable std::mutex mUnique;
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

