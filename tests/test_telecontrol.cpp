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
  BOOST_CHECK_NO_THROW(IEC104::Iec104Server(IEC104::Iec104ServerParameter::LocalDefaultServer(1)));
}

BOOST_AUTO_TEST_CASE(ShallSucceedBasicConnect)
{
  InitClient();
  IEC104::Iec104Server server(IEC104::Iec104ServerParameter::LocalDefaultServer(1));
  server.StartServer();
  BOOST_CHECK_EQUAL(gpTestClient->Connect(), true);
  KillClient();

  // New connect shall still succeed
  InitClient();
  BOOST_CHECK_EQUAL(gpTestClient->Connect(), true);
}

BOOST_AUTO_TEST_CASE(ShallSucceedCreate104DatapointDefinition)
{
  const IEC104::TypeIdEnum typeId(M_DP_NA_1); // -> enum label string
  const long dataId = M_DP_TB_1; // -> numeric string
  std::unique_ptr<const TC::BasePropertyList> pDoublePointDef(new IEC104::Iec104DataDefinition(typeId, dataId));

  BOOST_CHECK_EQUAL(pDoublePointDef->GetEnum<TypeID>(IEC104::Iec104DataDefinition::csTypeId), typeId);
  BOOST_CHECK_EQUAL(pDoublePointDef->GetString(IEC104::Iec104DataDefinition::csTypeId), typeId.GetString());

  BOOST_CHECK_EQUAL(pDoublePointDef->GetInt(IEC104::Iec104DataDefinition::csDataId), dataId);
  BOOST_CHECK_EQUAL(pDoublePointDef->GetString(IEC104::Iec104DataDefinition::csDataId), std::to_string(dataId));

  BOOST_CHECK_THROW(pDoublePointDef->GetInt("unknownKey"), std::invalid_argument);
  BOOST_CHECK_THROW(pDoublePointDef->GetEnum<TypeID>(IEC104::Iec104DataDefinition::csDataId), std::invalid_argument); // Invalid: Cannot convert "31" -> "M_DP_TB_1"
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

  BOOST_AUTO_TEST_CASE(ShallSucceedRegisterServerData)
  {
    const IEC104::TypeIdEnum stateType(M_DP_NA_1);
    const IEC104::TypeIdEnum controlType(M_DP_NA_1); // TODO: Implement control types

    const long dataId1 = 0x030044;
    const long dataId2 = 0x020044;
    const int serverId = 4;

    IEC104::Iec104DataDefinition stateDef(stateType, dataId1);
    IEC104::Iec104DataDefinition controlDef(controlType, dataId2);
    IEC104::Iec104Server server(IEC104::Iec104ServerParameter::LocalDefaultServer(serverId));
    
    BOOST_CHECK_NO_THROW(server.RegisterStatusData(stateDef));
    BOOST_CHECK_NO_THROW(server.RegisterControlData(controlDef));


    BOOST_CHECK_EQUAL(server.CountStatusData(), 1);
    BOOST_CHECK_EQUAL(server.CountControlData(), 1);

    BOOST_CHECK_NO_THROW(server.UnregisterData(stateDef.GetString(IEC104::Iec104DataDefinition::csDataId)));
    BOOST_CHECK_NO_THROW(server.UnregisterData(controlDef.GetString(IEC104::Iec104DataDefinition::csDataId)));
      
    BOOST_CHECK_EQUAL(server.CountStatusData(), 0);
    BOOST_CHECK_EQUAL(server.CountControlData(), 0);
  }



