/*
 * watchdog.hpp
 *
 *  Created on: 22.12.2019
 *      Author: Rene Buettgen
 */

#ifndef WATCHDOG_HPP_
#define WATCHDOG_HPP_

#include <mutex>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>

namespace UTIL
{

  class WatchDog
  {
  public:
    template <typename numericType, typename period>
    WatchDog(const std::chrono::duration<numericType, period>& arTimeout, const std::function<void()>& arTimeoutFunction)
    : SignalTimeout(arTimeoutFunction),
      mTimeout(std::chrono::duration_cast<std::chrono::milliseconds>(arTimeout)),
      mTick(std::chrono::milliseconds(50)),
      mpTimer(nullptr),
      mSignalKill(false) {}

    virtual ~WatchDog();

    void Start();
    void Stop(); //!< Stop the watchdog in time.

    WatchDog(const WatchDog&) = delete;
    WatchDog(WatchDog&&) = delete;
    WatchDog& operator=(const WatchDog&) = delete;
    WatchDog& operator=(WatchDog&&) = delete;

  private:
    void DoWatch();
    /// Thread safe write access to kill signal
    void SetKillSignal(bool aValue);
    /// Thread safe read access to kill signal
    bool IsKillRequested() const;

    std::function<void()> SignalTimeout; //!< Called if the watchdog is not stopped in time

    std::chrono::milliseconds mTimeout;
    std::chrono::milliseconds mTick;

    // thread management
    std::unique_ptr<std::thread> mpTimer;
    bool mSignalKill;
    mutable std::mutex mAccess;
  };

} /* namespace UTIL */

#endif /* WATCHDOG_HPP_ */
