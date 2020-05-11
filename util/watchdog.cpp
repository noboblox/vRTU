/*
 * watchdog.cpp
 *
 *  Created on: 22.12.2019
 *      Author: Rene Buettgen
 */

#include "watchdog.hpp"

namespace UTIL
{


  WatchDog::~WatchDog()
  {
    Stop(); // Join timer thread
  }

  void WatchDog::Start()
  {
    Stop();
    mpTimer.reset(new std::thread(&WatchDog::DoWatch, this));
  }

  void WatchDog::Stop()
  {
    if (mpTimer && mpTimer->joinable())
    {
      SetKillSignal(true);
      mpTimer->join();
      SetKillSignal(false);
    }
  }

  void WatchDog::SetKillSignal(bool aValue)
  {
    std::lock_guard<std::mutex> owner(mAccess);
    mSignalKill = aValue;
  }

  bool WatchDog::IsKillRequested() const
  {
    std::lock_guard<std::mutex> owner(mAccess);
    return mSignalKill;
  }

  void WatchDog::DoWatch()
  {
    const std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point now = start;
    std::chrono::milliseconds elapsed_ms(0);

    while(elapsed_ms < mTimeout)
    {
        if (IsKillRequested())
          return;

        now = std::chrono::system_clock::now();
        elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        std::this_thread::sleep_for(mTick);

    }
    SignalTimeout();
  }

} /* namespace UTIL */
