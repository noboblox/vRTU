/*
 * test_telecontrol.cc
 *
 *  Created on: 18.12.2019
 *      Author: Rene Buettgen
 */

#define BOOST_TEST_MODULE TestTelecontrol

#include <boost/test/unit_test.hpp>
#include "iec104server.hpp"

#include "mock_testclient.hpp"

BOOST_AUTO_TEST_CASE(ShallSucceedConstructServer)
{
  BOOST_CHECK_NO_THROW(TC::Iec104Server(TC::Iec104ServerParameter::LocalDefaultServer(1)));
}

BOOST_AUTO_TEST_CASE(ShallSucceedBasicConnect)
{
  InitClient();
  TC::Iec104Server server(TC::Iec104ServerParameter::LocalDefaultServer(1));
  server.StartServer();
  BOOST_CHECK_EQUAL(gpTestClient->Connect(), true);
  KillClient();

  // New connect shall still succeed
  InitClient();
  BOOST_CHECK_EQUAL(gpTestClient->Connect(), true);
}







