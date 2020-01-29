/*
 * test_telecontrol.cc
 *
 *  Created on: 18.12.2019
 *      Author: Rene Buettgen
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestTelecontrol

#include <boost/test/unit_test.hpp>

#include "staticenums.hpp"
#include "iec104server.hpp"
#include "iec104properties.hpp"
#include "iec104data.hpp"
#include "enumtype.hpp"
#include "iec104quality.hpp"
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
    const IEC104::TypeIdEnum controlType(M_DP_TA_1); // TODO: Implement control types

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

  BOOST_AUTO_TEST_CASE(SpInfoObjectPointerShallPreventMemoryLeaks)
  {
    {
      IEC104::SpInfoObjectPointer<DoublePointInformation> pDoublePoint(DoublePointInformation_create(nullptr, 1234, IEC60870_DOUBLE_POINT_INDETERMINATE, IEC60870_QUALITY_GOOD));
    } // Shall free()

    {
      IEC104::SpInfoObjectPointer<DoublePointInformation> pDoublePoint(DoublePointInformation_create(nullptr, 1234, IEC60870_DOUBLE_POINT_INDETERMINATE, IEC60870_QUALITY_GOOD));
      pDoublePoint.Reset(DoublePointInformation_create(nullptr, 5678, IEC60870_DOUBLE_POINT_INDETERMINATE, IEC60870_QUALITY_GOOD)); // Shall free() old and own new
    } // Shall free() new

    {
      IEC104::SpInfoObjectPointer<DoublePointInformation> pDoublePoint(DoublePointInformation_create(nullptr, 1234, IEC60870_DOUBLE_POINT_INDETERMINATE, IEC60870_QUALITY_GOOD));
      DoublePointInformation pRaw = pDoublePoint.Release(); // Release ownership
      DoublePointInformation_destroy(pRaw); // Shall free
    } // Shall not call free() again

#   if 0 // Shall not compile
    {
      IEC104::SpInfoObjectPointer<DoublePointInformation> pDoublePoint(DoublePointInformation_create(nullptr, 1234, IEC60870_DOUBLE_POINT_INDETERMINATE, IEC60870_QUALITY_GOOD));
      IEC104::SpInfoObjectPointer<DoublePointInformation> pCopy(pDoublePoint);
    }
#   endif
    {
      IEC104::SpInfoObjectPointer<DoublePointInformation> pDoublePoint(DoublePointInformation_create(nullptr, 1234, IEC60870_DOUBLE_POINT_INDETERMINATE, IEC60870_QUALITY_GOOD));
      IEC104::SpInfoObjectPointer<DoublePointInformation> pMove(std::move(pDoublePoint));
    } // pMove calls free()
  }

  BOOST_AUTO_TEST_CASE(CheckQualityFlagMapping)
  {
    BOOST_CHECK_EQUAL(IEC60870_QUALITY_GOOD,                 IEC104::Quality("good").GetInt());
    BOOST_CHECK_EQUAL(IEC60870_QUALITY_BLOCKED,              IEC104::Quality("blocked").GetInt());
    BOOST_CHECK_EQUAL(IEC60870_QUALITY_ELAPSED_TIME_INVALID, IEC104::Quality("timeout").GetInt());
    BOOST_CHECK_EQUAL(IEC60870_QUALITY_INVALID,              IEC104::Quality("invalid").GetInt());
    BOOST_CHECK_EQUAL(IEC60870_QUALITY_NON_TOPICAL,          IEC104::Quality("non-topical").GetInt());
    BOOST_CHECK_EQUAL(IEC60870_QUALITY_OVERFLOW,             IEC104::Quality("overflow").GetInt());
    BOOST_CHECK_EQUAL(IEC60870_QUALITY_RESERVED,             IEC104::Quality("reserved").GetInt());
    BOOST_CHECK_EQUAL(IEC60870_QUALITY_SUBSTITUTED,          IEC104::Quality("substituted").GetInt());

    BOOST_CHECK_THROW(IEC104::Quality("garbage"), std::invalid_argument);

    IEC104::Quality testQuality;
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_INVALID);
    testQuality.SetToGood();
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_GOOD);

    testQuality.EditFlagBlocked(true);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_BLOCKED);
    
    testQuality.EditFlagTimeout(true);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_BLOCKED | IEC60870_QUALITY_ELAPSED_TIME_INVALID);
    
    testQuality.EditFlagInvalid(true);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_BLOCKED | IEC60870_QUALITY_ELAPSED_TIME_INVALID | IEC60870_QUALITY_INVALID);
    
    testQuality.EditFlagNonTopical(true);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_BLOCKED | IEC60870_QUALITY_ELAPSED_TIME_INVALID | IEC60870_QUALITY_INVALID |
                                            IEC60870_QUALITY_NON_TOPICAL);
    
    testQuality.EditFlagOverflow(true);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_BLOCKED | IEC60870_QUALITY_ELAPSED_TIME_INVALID | IEC60870_QUALITY_INVALID |
                                            IEC60870_QUALITY_NON_TOPICAL | IEC60870_QUALITY_OVERFLOW);
    
    testQuality.EditFlagReserved(true);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_BLOCKED | IEC60870_QUALITY_ELAPSED_TIME_INVALID | IEC60870_QUALITY_INVALID |
                                            IEC60870_QUALITY_NON_TOPICAL | IEC60870_QUALITY_OVERFLOW | IEC60870_QUALITY_RESERVED);
    
    testQuality.EditFlagSubstituted(true);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_BLOCKED | IEC60870_QUALITY_ELAPSED_TIME_INVALID | IEC60870_QUALITY_INVALID |
                                            IEC60870_QUALITY_NON_TOPICAL | IEC60870_QUALITY_OVERFLOW | IEC60870_QUALITY_RESERVED |
                                            IEC60870_QUALITY_SUBSTITUTED);

    testQuality.EditFlagBlocked(false);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_ELAPSED_TIME_INVALID | IEC60870_QUALITY_INVALID | IEC60870_QUALITY_NON_TOPICAL |
                                            IEC60870_QUALITY_OVERFLOW | IEC60870_QUALITY_RESERVED | IEC60870_QUALITY_SUBSTITUTED);
    testQuality.EditFlagTimeout(false);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_INVALID | IEC60870_QUALITY_NON_TOPICAL | IEC60870_QUALITY_OVERFLOW |
                                            IEC60870_QUALITY_RESERVED |IEC60870_QUALITY_SUBSTITUTED);

    testQuality.EditFlagInvalid(false);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_NON_TOPICAL | IEC60870_QUALITY_OVERFLOW | IEC60870_QUALITY_RESERVED |
      IEC60870_QUALITY_SUBSTITUTED);

    testQuality.EditFlagNonTopical(false);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_OVERFLOW | IEC60870_QUALITY_RESERVED | IEC60870_QUALITY_SUBSTITUTED);

    testQuality.EditFlagOverflow(false);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_RESERVED | IEC60870_QUALITY_SUBSTITUTED);

    testQuality.EditFlagReserved(false);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_SUBSTITUTED);

    testQuality.EditFlagSubstituted(false);
    BOOST_CHECK_EQUAL(testQuality.GetInt(), IEC60870_QUALITY_GOOD);

    IEC104::Quality good(IEC60870_QUALITY_GOOD);
    BOOST_CHECK_EQUAL(good.IsGood(), true);
    BOOST_CHECK_EQUAL(good.IsInvalid(), false);

    IEC104::Quality good2("good");
    BOOST_CHECK_EQUAL(good == good2, true);
    good2.EditFlagInvalid(true);
    BOOST_CHECK_EQUAL(good != good2, true);
  }



