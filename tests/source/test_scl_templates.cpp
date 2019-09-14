#define BOOST_TEST_MODULE scl_templates
#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "enumtype.hpp"
#include "dataattribute.hpp"
#include "dataobject.hpp"

BOOST_AUTO_TEST_CASE(test_enumtype)
{
  SCL::EnumType enumtype("switching state");
  BOOST_REQUIRE_EQUAL(enumtype.GetTypeName(), "switching state");

  BOOST_REQUIRE_NO_THROW(enumtype.Insert(1, "open"));
  BOOST_REQUIRE_NO_THROW(enumtype.Insert(0, "intermediate"));
  BOOST_REQUIRE_NO_THROW(enumtype.Insert(2, "closed"));
  BOOST_REQUIRE_NO_THROW(enumtype.Insert(3, "faulty"));

  BOOST_REQUIRE_THROW(enumtype.Insert(3, "error1"), std::invalid_argument);    // error
  BOOST_REQUIRE_THROW(enumtype.Insert(233, "faulty"), std::invalid_argument);  // error


  BOOST_REQUIRE_EQUAL(enumtype.NameToValue("closed"), 2);
  BOOST_REQUIRE_THROW(enumtype.NameToValue("Closed"), std::out_of_range); // error
  
  BOOST_REQUIRE_EQUAL(enumtype.ValueToName(3), "faulty");
  BOOST_REQUIRE_THROW(enumtype.ValueToName(5), std::out_of_range); // error
}

BOOST_AUTO_TEST_CASE(test_data_attribute)
{
	SCL::BasicDataAttribute f_bda("f", SCL::BasicDataAttribute::FLOAT32); // basic BDA
	SCL::BasicDataAttribute i_bda("i", SCL::BasicDataAttribute::INT32); // basic BDA

	SCL::DataAttributeType analogue_val("analogue_value1"); // DAType
	analogue_val.Insert(f_bda);
	analogue_val.Insert(i_bda);

	SCL::BasicDataAttribute mag("mag", analogue_val); // constructed BDA
	SCL::BasicDataAttribute ang("ang", analogue_val); // constructed BDA
	SCL::DataAttributeType scl_vector("vector1"); // DAType
	scl_vector.Insert(mag);
	scl_vector.Insert(ang);

	SCL::DataAttribute cVal("cVal", scl_vector, SCL::DataAttribute::MX, SCL::DataAttribute::dchg); // constructed DA
	SCL::DataAttribute t("t", SCL::BasicDataAttribute::Timestamp, SCL::DataAttribute::MX, SCL::DataAttribute::no_triggers); // basic DA


	SCL::EnumType originEnum("orCategory");

	SCL::BasicDataAttribute orCat("orCat", originEnum); // enum BDA
	SCL::DataAttribute enum_da("DA_ENUM", originEnum, SCL::DataAttribute::CF, SCL::DataAttribute::dupd_qchg); // fictional enum DA


	BOOST_REQUIRE_EQUAL(enum_da.HasTriggerOnDataChange(), false);
	BOOST_REQUIRE_EQUAL(enum_da.HasTriggerOnDataUpdate(), true);
	BOOST_REQUIRE_EQUAL(enum_da.HasTriggerOnQualityChange(), true);
	
	BOOST_REQUIRE_EQUAL(t.HasTriggerOnDataChange(), false);
	BOOST_REQUIRE_EQUAL(t.HasTriggerOnDataUpdate(), false);
	BOOST_REQUIRE_EQUAL(t.HasTriggerOnQualityChange(), false);

	BOOST_REQUIRE_EQUAL(f_bda.IsBasicType(), true);
	BOOST_REQUIRE_EQUAL(f_bda.IsEnum(), false);
	BOOST_REQUIRE_EQUAL(f_bda.IsStruct(), false);
		
	BOOST_REQUIRE_EQUAL(cVal.IsBasicType(), false);
	BOOST_REQUIRE_EQUAL(cVal.IsEnum(), false);
	BOOST_REQUIRE_EQUAL(cVal.IsStruct(), true);
}

BOOST_AUTO_TEST_CASE(test_data_object)
{
	// Preconditions
	SCL::BasicDataAttribute f_bda("f", SCL::BasicDataAttribute::FLOAT32); // basic BDA
	SCL::BasicDataAttribute i_bda("i", SCL::BasicDataAttribute::INT32); // basic BDA

	SCL::DataAttributeType analogue_val("analogue_value1"); // DAType
	analogue_val.Insert(f_bda);
	analogue_val.Insert(i_bda);

	SCL::BasicDataAttribute mag("mag", analogue_val); // constructed BDA
	SCL::BasicDataAttribute ang("ang", analogue_val); // constructed BDA
	SCL::DataAttributeType scl_vector("vector1"); // DAType
	scl_vector.Insert(mag);
	scl_vector.Insert(ang);

	SCL::DataAttribute cVal("cVal", scl_vector, SCL::DataAttribute::MX, SCL::DataAttribute::dchg); // constructed DA
	SCL::DataAttribute t("t", SCL::DataAttribute::Timestamp, SCL::DataAttribute::MX, SCL::DataAttribute::no_triggers); // basic DA
	SCL::DataAttribute q("q", SCL::DataAttribute::Quality, SCL::DataAttribute::MX, SCL::DataAttribute::qchg); // basic DA

	// Test DOs
	SCL::DataObjectType cmv("CMV_1", SCL::DataObjectType::CMV); // DOType
	cmv.Insert(cVal); // insert DA
	cmv.Insert(t); // insert DA
	cmv.Insert(q); // insert DA

	SCL::SubDataObject phsA("phsA", cmv); // SDO
	SCL::SubDataObject phsB("phsB", cmv); // SDO
	SCL::SubDataObject phsC("phsC", cmv); // SDO

	SCL::DataObjectType wye("WYE_1", SCL::DataObjectType::WYE); // DOType
	wye.Insert(phsA); // insert SDO
	wye.Insert(phsB); // insert SDO
	wye.Insert(phsC); // insert SDO



	// Data objects

}