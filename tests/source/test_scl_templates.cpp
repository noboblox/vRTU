#define BOOST_TEST_MODULE scl_templates
#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "enumtype.hpp"
#include "dataattribute.hpp"

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
	BOOST_REQUIRE_NO_THROW(SCL::BasicDataAttribute f_bda("f", SCL::BasicDataAttribute::FLOAT32)); // Basic type
	BOOST_REQUIRE_NO_THROW(SCL::BasicDataAttribute i_bda("i", SCL::BasicDataAttribute::INT32));
	
	BOOST_REQUIRE_NO_THROW(SCL::DataAttributeType analogue_val("analogue_value1"));
	analogue_val.Insert(f_bda);
    analogue_val.Insert(i_bda);

	BOOST_REQUIRE_NO_THROW(SCL::BasicDataAttribute mag("mag", analogue_val));
	BOOST_REQUIRE_NO_THROW(SCL::BasicDataAttribute ang("ang", analogue_val));
	BOOST_REQUIRE_NO_THROW(SCL::DataAttributeType vector("vector1"));
	vector.Insert(mag);
	vector.Insert(ang);

	BOOST_REQUIRE_NO_THROW(SCL::DataAttribute cVal("cVal", vector, SCL::DataAttribute::MX, SCL::DataAttribute::dchg));
	BOOST_REQUIRE_NO_THROW(SCL::DataAttribute t("t", SCL::BasicDataAttribute::Timestamp, SCL::DataAttribute::MX, SCL::DataAttribute::no_triggers));
}