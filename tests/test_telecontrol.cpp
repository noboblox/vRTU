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
#include "enumtype.hpp"
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
  const TC::TypeIdEnum typeId(M_DP_NA_1); // -> enum label string
  const long dataId = M_DP_TB_1; // -> numeric string
  std::unique_ptr<const TC::BasePropertyList> pDoublePointDef(new TC::Iec104DataDefinition(typeId, dataId));

  BOOST_CHECK_EQUAL(pDoublePointDef->GetEnum<TypeID>(TC::Iec104DataDefinition::csTypeId), typeId);
  BOOST_CHECK_EQUAL(pDoublePointDef->GetString(TC::Iec104DataDefinition::csTypeId), typeId.GetString());

  BOOST_CHECK_EQUAL(pDoublePointDef->GetInt(TC::Iec104DataDefinition::csDataId), dataId);
  BOOST_CHECK_EQUAL(pDoublePointDef->GetString(TC::Iec104DataDefinition::csDataId), std::to_string(dataId));

  BOOST_CHECK_THROW(pDoublePointDef->GetInt("unknownKey"), std::invalid_argument);
  BOOST_CHECK_THROW(pDoublePointDef->GetEnum<TypeID>(TC::Iec104DataDefinition::csDataId), std::invalid_argument); // Invalid: Cannot convert "31" -> "M_DP_TB_1"
}

// Define a test enum
enum Test
{
  ONE = 1,
  TWO = 2,
  THREE = 3
};

template<>
UTIL::Enum<Test>::EnumDefinition const UTIL::Enum<Test>::msDefinition
{
  {"ONE", ONE},
  {"TWO", TWO},
  {"THREE", THREE}
};

  BOOST_AUTO_TEST_CASE(ShallSucceedCheckEnumType)
{  
  // See enum definition above
  UTIL::Enum<Test> testValue("ONE");
  BOOST_CHECK_EQUAL(testValue.GetValue(), ONE);
  BOOST_CHECK_EQUAL(testValue.GetString(), "ONE");

  testValue = "TWO";
  BOOST_CHECK_EQUAL(testValue.GetValue(), TWO);
  BOOST_CHECK_EQUAL(testValue.GetString(), "TWO");

  testValue = THREE;
  BOOST_CHECK_EQUAL(testValue.GetValue(), THREE);
  BOOST_CHECK_EQUAL(testValue.GetString(), "THREE");

  BOOST_CHECK_THROW(UTIL::Enum<Test> invalidValue("FOUR"), std::invalid_argument);

  testValue = "TWO";
  UTIL::Enum<Test> compare("TWO");
  BOOST_CHECK_EQUAL(testValue == compare, true);
  BOOST_CHECK_EQUAL(testValue != compare, false);
  BOOST_CHECK_EQUAL(testValue <= compare, true);
  BOOST_CHECK_EQUAL(testValue >= compare, true);
  BOOST_CHECK_EQUAL(testValue <  compare, false);
  BOOST_CHECK_EQUAL(testValue >  compare, false);
  
  compare = "THREE";
  BOOST_CHECK_EQUAL(testValue == compare, false);
  BOOST_CHECK_EQUAL(testValue != compare, true);
  BOOST_CHECK_EQUAL(testValue <= compare, true);
  BOOST_CHECK_EQUAL(testValue >= compare, false);
  BOOST_CHECK_EQUAL(testValue <  compare, true);
  BOOST_CHECK_EQUAL(testValue >  compare, false);

  compare = "ONE";
  BOOST_CHECK_EQUAL(testValue == compare, false);
  BOOST_CHECK_EQUAL(testValue != compare, true);
  BOOST_CHECK_EQUAL(testValue <= compare, false);
  BOOST_CHECK_EQUAL(testValue >= compare, true);
  BOOST_CHECK_EQUAL(testValue <  compare, false);
  BOOST_CHECK_EQUAL(testValue >  compare, true);
  }




