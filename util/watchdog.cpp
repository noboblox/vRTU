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
      mKill = true;
      mpTimer->join();
      mKill = false;
    }
  }

  void WatchDog::DoWatch()
  {
    const std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point now = start;
    std::chrono::milliseconds elapsedMs(0);

    while(elapsedMs < mTimeout)
    {
        if (mKill)
          return;

        now = std::chrono::system_clock::now();
        elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        std::this_thread::sleep_for(mTick);

    }
    SignalTimeout();
  }

} /* namespace UTIL */
