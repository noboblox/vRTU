/*
 * test_telecontrol.cc
 *
 *  Created on: 18.12.2019
 *      Author: Rene Buettgen
 */

#define BOOST_TEST_MODULE TestTelecontrol

#include <boost/test/unit_test.hpp>
#include <set>

#include "staticenums.hpp"
#include "iec104server.hpp"
#include "iec104properties.hpp"
#include "iec104data.hpp"
#include "enumtype.hpp"
#include "iec104quality.hpp"
#include "iec104timestamp.hpp"

BOOST_AUTO_TEST_CASE(ShallSucceedConstructServer)
{
  BOOST_CHECK_NO_THROW(IEC104::Iec104Server(IEC104::Iec104ServerParameter::LocalDefaultServer(1)));
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
    const IEC104::TypeIdEnum controlType(M_DP_TB_1); // TODO: Implement control types

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

  BOOST_AUTO_TEST_CASE(ShallSucceedStartStopServer)
  {
    IEC104::Iec104Server server(IEC104::Iec104ServerParameter::LocalDefaultServer(1234));
    BOOST_CHECK_NO_THROW(server.StartServer()); // localhost:2404

    IEC104::Iec104Server conflicing_server(IEC104::Iec104ServerParameter::LocalDefaultServer(3456));
    BOOST_CHECK_THROW(conflicing_server.StartServer(), std::runtime_error); // localhost:2404 is occupied

    BOOST_CHECK_NO_THROW(server.StartServer()); // Should do nothing
    BOOST_CHECK_NO_THROW(server.StopServer());

    // Restart
    BOOST_CHECK_NO_THROW(server.StartServer());
    BOOST_CHECK_NO_THROW(server.StopServer());
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

  BOOST_AUTO_TEST_CASE(TestDoublePointDataHierarchy)
  {
    const int ioaSize = 3;
    IEC104::DataFactory factory(ioaSize);

    {
      const IEC104::Iec104DataDefinition defDoublePointNoTimestamp(IEC104::TypeIdEnum(M_DP_NA_1), 1234);

      auto spDoublePoint = factory.Create(defDoublePointNoTimestamp);
      BOOST_CHECK_EQUAL(spDoublePoint->GetTypeString(), "M_DP_NA_1");
      BOOST_CHECK_EQUAL(spDoublePoint->GetId(), "1234");
      BOOST_CHECK_EQUAL(spDoublePoint->GetValue(), "faulty");
      BOOST_CHECK_EQUAL(spDoublePoint->GetQuality(), "invalid");
      BOOST_CHECK_EQUAL(spDoublePoint->GetTimestamp(), "");

      IEC104::SpBaseInformation pNative(spDoublePoint->Write());
      BOOST_CHECK_EQUAL(DoublePointInformation_getValue(reinterpret_cast<DoublePointInformation>(pNative.Get())), IEC60870_DOUBLE_POINT_INDETERMINATE);
      BOOST_CHECK_EQUAL(DoublePointInformation_getQuality(reinterpret_cast<DoublePointInformation>(pNative.Get())), IEC60870_QUALITY_INVALID);
      BOOST_CHECK_EQUAL(InformationObject_getType(pNative.Get()), M_DP_NA_1);
      BOOST_CHECK_EQUAL(InformationObject_getObjectAddress(pNative.Get()), 1234);
    }

    {
      const IEC104::Iec104DataDefinition defDoublePointTime56(IEC104::TypeIdEnum(M_DP_TB_1), 1235);
      auto spDoublePointTime = factory.Create(defDoublePointTime56);
      BOOST_CHECK_EQUAL(spDoublePointTime->GetTypeString(), "M_DP_TB_1");
      BOOST_CHECK_EQUAL(spDoublePointTime->GetId(), "1235");
      BOOST_CHECK_EQUAL(spDoublePointTime->GetValue(), "faulty");
      BOOST_CHECK_EQUAL(spDoublePointTime->GetQuality(), "invalid");
      BOOST_CHECK_NE(spDoublePointTime->GetTimestamp(), "");

      TC::BaseDataPropertyList updateData("on", "good", "2020-01-23T18:05:23.134");
      const size_t utcTime = 1579802723134;

      BOOST_CHECK_NO_THROW(spDoublePointTime->Update(updateData));
      IEC104::SpBaseInformation pNative(spDoublePointTime->Write());
      BOOST_CHECK_EQUAL(DoublePointInformation_getValue(reinterpret_cast<DoublePointInformation>(pNative.Get())), IEC60870_DOUBLE_POINT_ON);
      BOOST_CHECK_EQUAL(DoublePointInformation_getQuality(reinterpret_cast<DoublePointInformation>(pNative.Get())), IEC60870_QUALITY_GOOD);
      CP56Time2a pTime = DoublePointWithCP56Time2a_getTimestamp(reinterpret_cast<DoublePointWithCP56Time2a>(pNative.Get()));
      BOOST_CHECK_EQUAL(CP56Time2a_toMsTimestamp(pTime), utcTime);
      BOOST_CHECK_EQUAL(InformationObject_getType(pNative.Get()), M_DP_TB_1);
      BOOST_CHECK_EQUAL(InformationObject_getObjectAddress(pNative.Get()), 1235);
    }

  }

  BOOST_AUTO_TEST_CASE(TestTimestamp)
  {
    const std::string shortTimestamp = "2019-09-23T14:23:00";
    const size_t utcShort = 1569248580000;
    const std::string longTimestamp = "2019-09-23T14:23:00.109";
    const size_t utcLong = 1569248580109;

    std::unique_ptr<IEC104::Timestamp> pTime = nullptr;
    BOOST_CHECK_NO_THROW(pTime.reset(new IEC104::Timestamp(shortTimestamp)));
    BOOST_CHECK_EQUAL(pTime->GetMilliseconds(), utcShort);
    BOOST_CHECK_NO_THROW(pTime.reset(new IEC104::Timestamp(longTimestamp)));
    BOOST_CHECK_EQUAL(pTime->GetMilliseconds(), utcLong);
  }

  BOOST_AUTO_TEST_CASE(TestSinglePointDataHierarchy)
  {
    const int ioaSize = 3;
    IEC104::DataFactory factory(ioaSize);

    {
      const IEC104::Iec104DataDefinition defSinglePointNoTimestamp(IEC104::TypeIdEnum(M_SP_NA_1), 1234);

      auto spSinglePoint = factory.Create(defSinglePointNoTimestamp);
      BOOST_CHECK_EQUAL(spSinglePoint->GetTypeString(), "M_SP_NA_1");
      BOOST_CHECK_EQUAL(spSinglePoint->GetId(), "1234");
      BOOST_CHECK_EQUAL(spSinglePoint->GetValue(), "false");
      BOOST_CHECK_EQUAL(spSinglePoint->GetQuality(), "invalid");
      BOOST_CHECK_EQUAL(spSinglePoint->GetTimestamp(), "");

      IEC104::SpBaseInformation pNative(spSinglePoint->Write());
      BOOST_CHECK_EQUAL(SinglePointInformation_getValue(reinterpret_cast<SinglePointInformation>(pNative.Get())), false);
      BOOST_CHECK_EQUAL(SinglePointInformation_getQuality(reinterpret_cast<SinglePointInformation>(pNative.Get())), IEC60870_QUALITY_INVALID);
      BOOST_CHECK_EQUAL(InformationObject_getType(pNative.Get()), M_SP_NA_1);
      BOOST_CHECK_EQUAL(InformationObject_getObjectAddress(pNative.Get()), 1234);
    }

    {
      const IEC104::Iec104DataDefinition defSinglePointTime56(IEC104::TypeIdEnum(M_SP_TB_1), 1235);
      auto spSinglePointTime = factory.Create(defSinglePointTime56);
      BOOST_CHECK_EQUAL(spSinglePointTime->GetTypeString(), "M_SP_TB_1");
      BOOST_CHECK_EQUAL(spSinglePointTime->GetId(), "1235");
      BOOST_CHECK_EQUAL(spSinglePointTime->GetValue(), "false");
      BOOST_CHECK_EQUAL(spSinglePointTime->GetQuality(), "invalid");
      BOOST_CHECK_NE(   spSinglePointTime->GetTimestamp(), "");

      TC::BaseDataPropertyList updateData("true", "blocked", "2020-01-23T18:05:23.134");
      const size_t utcTime = 1579802723134;

      BOOST_CHECK_NO_THROW(spSinglePointTime->Update(updateData));
      IEC104::SpBaseInformation pNative(spSinglePointTime->Write());
      BOOST_CHECK_EQUAL(SinglePointInformation_getValue(reinterpret_cast<SinglePointInformation>(pNative.Get())), true);
      BOOST_CHECK_EQUAL(SinglePointInformation_getQuality(reinterpret_cast<SinglePointInformation>(pNative.Get())), IEC60870_QUALITY_BLOCKED);
      CP56Time2a pTime = SinglePointWithCP56Time2a_getTimestamp(reinterpret_cast<SinglePointWithCP56Time2a>(pNative.Get()));
      BOOST_CHECK_EQUAL(CP56Time2a_toMsTimestamp(pTime), utcTime);
      BOOST_CHECK_EQUAL(InformationObject_getType(pNative.Get()), M_SP_TB_1);
      BOOST_CHECK_EQUAL(InformationObject_getObjectAddress(pNative.Get()), 1235);
    }

  }

  BOOST_AUTO_TEST_CASE(TestFloatDataHierarchy)
  {
    const int ioaSize = 3;
    IEC104::DataFactory factory(ioaSize);

    {
      const IEC104::Iec104DataDefinition defFloatNoTimestamp(IEC104::TypeIdEnum(M_ME_NC_1), 1234);

      auto spFloat = factory.Create(defFloatNoTimestamp);
      BOOST_CHECK_EQUAL(spFloat->GetTypeString(), "M_ME_NC_1");
      BOOST_CHECK_EQUAL(spFloat->GetId(), "1234");
      BOOST_CHECK_EQUAL(spFloat->GetValue(), std::to_string(float(0.0)));
      BOOST_CHECK_EQUAL(spFloat->GetQuality(), "invalid");
      BOOST_CHECK_EQUAL(spFloat->GetTimestamp(), "");

      IEC104::SpBaseInformation pNative(spFloat->Write());
      BOOST_CHECK_EQUAL(MeasuredValueShort_getValue(reinterpret_cast<MeasuredValueShort>(pNative.Get())), float(0.0));
      BOOST_CHECK_EQUAL(MeasuredValueShort_getQuality(reinterpret_cast<MeasuredValueShort>(pNative.Get())), IEC60870_QUALITY_INVALID);
      BOOST_CHECK_EQUAL(InformationObject_getType(pNative.Get()), M_ME_NC_1);
      BOOST_CHECK_EQUAL(InformationObject_getObjectAddress(pNative.Get()), 1234);
    }

    {
      const IEC104::Iec104DataDefinition defFloatTime56(IEC104::TypeIdEnum(M_ME_TF_1), 1235);
      auto spFloatTime = factory.Create(defFloatTime56);
      BOOST_CHECK_EQUAL(spFloatTime->GetTypeString(), "M_ME_TF_1");
      BOOST_CHECK_EQUAL(spFloatTime->GetId(), "1235");
      BOOST_CHECK_EQUAL(spFloatTime->GetValue(), std::to_string(float(0.0)));
      BOOST_CHECK_EQUAL(spFloatTime->GetQuality(), "invalid");
      BOOST_CHECK_NE(   spFloatTime->GetTimestamp(), "");

      TC::BaseDataPropertyList updateData("212.13", "overflow", "2020-01-23T18:05:23.134");
      const size_t utcTime = 1579802723134;

      BOOST_CHECK_NO_THROW(spFloatTime->Update(updateData));
      IEC104::SpBaseInformation pNative(spFloatTime->Write());
      BOOST_CHECK_EQUAL(MeasuredValueShort_getValue(reinterpret_cast<MeasuredValueShort>(pNative.Get())), float(212.13));
      BOOST_CHECK_EQUAL(MeasuredValueShort_getQuality(reinterpret_cast<MeasuredValueShort>(pNative.Get())), IEC60870_QUALITY_OVERFLOW);
      CP56Time2a pTime = MeasuredValueShortWithCP56Time2a_getTimestamp(reinterpret_cast<MeasuredValueShortWithCP56Time2a>(pNative.Get()));
      BOOST_CHECK_EQUAL(CP56Time2a_toMsTimestamp(pTime), utcTime);
      BOOST_CHECK_EQUAL(InformationObject_getType(pNative.Get()), M_ME_TF_1);
      BOOST_CHECK_EQUAL(InformationObject_getObjectAddress(pNative.Get()), 1235);
    }

  }

