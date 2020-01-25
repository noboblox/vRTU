/*
 * test_telecontrol.cc
 *
 *  Created on: 18.12.2019
 *      Author: Rene Buettgen
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestTelecontrol

#include <boost/test/unit_test.hpp>
#include "iec104server.hpp"
#include "iec104properties.hpp"

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

BOOST_AUTO_TEST_CASE(ShallSucceedCreate104DatapointDefinition)
{
  const long typeId = M_DP_NA_1;
  const long dataId = 5100;
  std::unique_ptr<const TC::BasePropertyList> pDoublePointDef(new TC::Iec104DataDefiniton(typeId, dataId));

  BOOST_CHECK_EQUAL(pDoublePointDef->GetInt(TC::Iec104DataDefiniton::csTypeId), typeId);
  BOOST_CHECK_EQUAL(pDoublePointDef->GetString(TC::Iec104DataDefiniton::csTypeId), std::to_string(typeId));

  BOOST_CHECK_EQUAL(pDoublePointDef->GetInt(TC::Iec104DataDefiniton::csDataId), dataId);
  BOOST_CHECK_EQUAL(pDoublePointDef->GetString(TC::Iec104DataDefiniton::csDataId), std::to_string(dataId));

  BOOST_CHECK_THROW(pDoublePointDef->GetInt("unknownKey"), std::invalid_argument);
}





