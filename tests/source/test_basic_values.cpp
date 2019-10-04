#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <iostream>
#include <memory>

#include "basicvalue.hpp"
#include "basicvaluefactory.hpp"
#include "dataattribute.hpp"


using namespace SCL;

BOOST_AUTO_TEST_CASE(test_basic_value_create)
{
  BasicValueFactory factory;

  std::vector<std::unique_ptr<BasicValueBase>> values;

  for (int i = static_cast<int>(SCL::BasicTypeId::BasicType_begin); i < static_cast<int>(SCL::BasicTypeId::BasicType_end); ++i)
  {
    factory.Create(static_cast<SCL::BasicTypeId::Type>(i));
  }

}
