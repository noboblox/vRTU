#define BOOST_TEST_MODULE scl_templates
#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "enumtype.hpp"
#include "dataattribute.hpp"
#include "dataobject.hpp"
#include "objectreference.hpp"
#include "enums.hpp"
#include "datatemplatecontainer.hpp"

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


	BOOST_REQUIRE_THROW(SCL::BasicDataAttribute("error1", SCL::DataAttributeBase::BasicType_enum), std::invalid_argument);
	BOOST_REQUIRE_THROW(SCL::BasicDataAttribute("error1", SCL::DataAttributeBase::BasicType_struct), std::invalid_argument);

	SCL::DataAttributeType analogue_val("analogue_value1"); // DAType
	BOOST_REQUIRE_EQUAL(analogue_val.GetTypeName(), "analogue_value1");
	analogue_val.Insert(f_bda);
	analogue_val.Insert(i_bda);

	SCL::BasicDataAttribute mag("mag", analogue_val); // constructed BDA
	SCL::BasicDataAttribute ang("ang", analogue_val); // constructed BDA
	BOOST_REQUIRE_EQUAL(mag.GetTypeName(), "analogue_value1");
	BOOST_REQUIRE_EQUAL(mag.GetName(), "mag");

	SCL::DataAttributeType scl_vector("vector1"); // DAType
	scl_vector.Insert(mag);
	scl_vector.Insert(ang);

	SCL::DataAttribute cVal("cVal", scl_vector,SCL::FunctionalConstraint::MX, SCL::DataAttribute::dchg); // constructed DA
	SCL::DataAttribute t("t", SCL::BasicDataAttribute::Timestamp,SCL::FunctionalConstraint::MX, SCL::DataAttribute::no_triggers); // basic DA
	BOOST_REQUIRE_EQUAL(cVal.GetTypeName(), "vector1");
	BOOST_REQUIRE_EQUAL(cVal.GetName(), "cVal");


	SCL::EnumType originEnum("orCategory");
	BOOST_REQUIRE_EQUAL(originEnum.GetTypeName(), "orCategory");

	SCL::BasicDataAttribute orCat("orCat", originEnum); // enum BDA
	SCL::DataAttribute enum_da("DA_ENUM", originEnum,SCL::FunctionalConstraint::CF, SCL::DataAttribute::dupd_qchg); // fictional enum DA


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

	SCL::DataAttribute cVal("cVal", scl_vector,SCL::FunctionalConstraint::MX, SCL::DataAttribute::dchg); // constructed DA
	SCL::DataAttribute t("t", SCL::DataAttribute::Timestamp,SCL::FunctionalConstraint::MX, SCL::DataAttribute::no_triggers); // basic DA
	SCL::DataAttribute q("q", SCL::DataAttribute::Quality,SCL::FunctionalConstraint::MX, SCL::DataAttribute::qchg); // basic DA

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
}

BOOST_AUTO_TEST_CASE(test_data_template_container)
{
	try
	{
		// This is the storage container
		SCL::DataTemplateContainer type_registry;

		// Preconditions DAType
		SCL::BasicDataAttribute f_bda("f", SCL::BasicDataAttribute::FLOAT32); // basic BDA
		SCL::BasicDataAttribute i_bda("i", SCL::BasicDataAttribute::INT32); // basic BDA

		auto& anval = type_registry.Manage(std::make_unique<SCL::DataAttributeType>("analogue_value1"));
		BOOST_REQUIRE_THROW(type_registry.Manage(std::make_unique<SCL::DataAttributeType>("analogue_value1")), std::invalid_argument); // Error: duplicate
		BOOST_REQUIRE_THROW(type_registry.Manage(std::unique_ptr<SCL::DataAttributeType>(nullptr)), std::invalid_argument); // Error: nullptr

		anval.Insert(f_bda);
		anval.Insert(i_bda);

		SCL::BasicDataAttribute mag("mag", anval); // constructed BDA
		SCL::BasicDataAttribute ang("ang", anval); // constructed BDA

		auto& scl_vector = type_registry.Manage(std::make_unique<SCL::DataAttributeType>("vector1")); // DAType
		scl_vector.Insert(mag);
		scl_vector.Insert(ang);

		SCL::DataAttribute cVal("cVal", scl_vector, SCL::FunctionalConstraint::MX, SCL::DataAttribute::dchg); // constructed DA
		SCL::DataAttribute t("t", SCL::DataAttribute::Timestamp, SCL::FunctionalConstraint::MX, SCL::DataAttribute::no_triggers); // basic DA
		SCL::DataAttribute q("q", SCL::DataAttribute::Quality, SCL::FunctionalConstraint::MX, SCL::DataAttribute::qchg); // basic DA

		// DOType
		auto& cmv = type_registry.Manage(std::make_unique<SCL::DataObjectType>("CMV_1", SCL::DataObjectType::CMV)); // DOType
		BOOST_REQUIRE_THROW(type_registry.Manage(std::make_unique<SCL::DataObjectType>("CMV_1", SCL::DataObjectType::CMV)), std::invalid_argument); // Error: duplicate
		BOOST_REQUIRE_THROW(type_registry.Manage(std::unique_ptr<SCL::DataObjectType>(nullptr)), std::invalid_argument); // Error: nullptr

		cmv.Insert(cVal); // insert DA
		cmv.Insert(t); // insert DA
		cmv.Insert(q); // insert DA

		SCL::SubDataObject phsA("phsA", cmv); // SDO
		SCL::SubDataObject phsB("phsB", cmv); // SDO
		SCL::SubDataObject phsC("phsC", cmv); // SDO

		auto& wye = type_registry.Manage(std::make_unique<SCL::DataObjectType>("WYE_1", SCL::DataObjectType::WYE)); // DOType
		wye.Insert(phsA); // insert SDO
		wye.Insert(phsB); // insert SDO
		wye.Insert(phsC); // insert SDO


		// Precondition EnumType
		auto& enumtype = type_registry.Manage(std::make_unique<SCL::EnumType>("switching state"));
		

		// Check Getter
		BOOST_REQUIRE_NO_THROW(type_registry.GetEnumType("switching state"));
		BOOST_REQUIRE_NO_THROW(type_registry.GetDOType("WYE_1"));
		BOOST_REQUIRE_NO_THROW(type_registry.GetDOType("CMV_1"));
		BOOST_REQUIRE_NO_THROW(type_registry.GetDAType("vector1"));
		BOOST_REQUIRE_NO_THROW(type_registry.GetDAType("analogue_value1"));
		
		BOOST_REQUIRE_THROW(type_registry.GetDAType("CMV_1"), std::out_of_range); // error
		BOOST_REQUIRE_THROW(type_registry.GetEnumType("CMV_1"), std::out_of_range); // error

		return;
	}
	catch (std::exception& e) { std::cout << e.what() << std::endl; }
	BOOST_FAIL("Fail: Unplanned exception occured.");
}


BOOST_AUTO_TEST_CASE(object_reference)
{
	SCL::ObjectReference ref1("QA1CSWI1.Pos.stVal"); // 3 layers
	SCL::ObjectReference ref2("QA1CSWI1$Pos$stVal", SCL::FunctionalConstraint::undefined, '$'); // 3 layers alternative seperator
	SCL::ObjectReference ref3("Q02MMXU1.A.phsA.cVal.mag.f"); // 6 layers no fc
	SCL::ObjectReference ref4("Q02MMXU1.A.phsA.cVal.mag.f", SCL::FunctionalConstraint::MX); // 6 layers fc=MX

	BOOST_REQUIRE_EQUAL(ref1.GetLayer(), 3);
	BOOST_REQUIRE_EQUAL(ref2.GetLayer(), 3);
	BOOST_REQUIRE_EQUAL(ref3.GetLayer(), 6);
	BOOST_REQUIRE_EQUAL(ref4.GetLayer(), 6);

	BOOST_REQUIRE_EQUAL(ref1.GetElementName(1), "QA1CSWI1");
	BOOST_REQUIRE_EQUAL(ref1.GetElementName(2), "Pos");
	BOOST_REQUIRE_EQUAL(ref1.GetElementName(3), "stVal");
	
	BOOST_REQUIRE_EQUAL(ref2.GetElementName(1), "QA1CSWI1");
	BOOST_REQUIRE_EQUAL(ref2.GetElementName(2), "Pos");
	BOOST_REQUIRE_EQUAL(ref2.GetElementName(3), "stVal");
	
	BOOST_REQUIRE_EQUAL(ref3.GetElementName(1), "Q02MMXU1");
	BOOST_REQUIRE_EQUAL(ref3.GetElementName(2), "A");
	BOOST_REQUIRE_EQUAL(ref3.GetElementName(3), "phsA");
	BOOST_REQUIRE_EQUAL(ref3.GetElementName(4), "cVal");
	BOOST_REQUIRE_EQUAL(ref3.GetElementName(5), "mag");
	BOOST_REQUIRE_EQUAL(ref3.GetElementName(6), "f");
}



