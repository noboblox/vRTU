/*
 * test_datamodel.cpp
 *
 *  Created on: 22.12.2019
 *      Author: rene
 */

#include <boost/test/unit_test.hpp>

#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
#include "watchdog.hpp"

BOOST_AUTO_TEST_CASE(ShallSucceedWatchdogTimeout)
{
  std::cout << "Test if watchdog times out after 1s: " << std::endl;

  std::atomic<bool> hasTimedOut(false);
  UTIL::WatchDog oneSecTimeout(std::chrono::seconds(1), [&hasTimedOut](){hasTimedOut = true;});

  const auto start = std::chrono::system_clock::now();
  auto stop = start;

  oneSecTimeout.Start();
  while (!hasTimedOut)
  {
    stop = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  auto timeoutDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  const auto maxDuration = std::chrono::milliseconds(1250);
  const auto minDuration = std::chrono::milliseconds(750);

  std::cout << "Duration = " << timeoutDuration.count() << "ms" << std::endl;
  if ((timeoutDuration > maxDuration) ||
      (timeoutDuration < minDuration))
  {
    BOOST_FAIL("Unexpected timeout duration");
  }
  std::cout << "Timeout test successful." << std::endl;
}

BOOST_AUTO_TEST_CASE(ShallSucceedWatchdogStartStop)
{
  std::cout << "Test watchdog Start/Stop: " << std::endl;
  std::atomic<bool> hasTimedOut(false);
  UTIL::WatchDog watchdog(std::chrono::milliseconds(100), [&hasTimedOut](){hasTimedOut = true;});

  watchdog.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  watchdog.Stop();

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  if (hasTimedOut)
  {
    BOOST_FAIL("Unexpected timeout");
  }
  std::cout << "Single Start/Stop successful " << std::endl
            << "Test Start() thread safety" << std::endl;

  watchdog.Start();
  watchdog.Start(); // reset
  watchdog.Start(); // reset
  watchdog.Start(); // reset
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  watchdog.Stop();

  if (hasTimedOut)
  {
    BOOST_FAIL("Unexpected timeout");
  }
  std::cout << "Test successful" << std::endl;

}
