#include "gtest/gtest.h"

#include <stdexcept>

#include "../SCL/enumtype.hpp"

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(TestEnumType, TestSCL)
{
  SCL::EnumType enumtype("switching state");
  EXPECT_EQ(enumtype.GetTypeName(), "switching state");

  EXPECT_NO_THROW(enumtype.Insert(1, "open"));
  EXPECT_NO_THROW(enumtype.Insert(0, "intermediate"));
  EXPECT_NO_THROW(enumtype.Insert(2, "closed"));
  EXPECT_NO_THROW(enumtype.Insert(3, "faulty"));

  EXPECT_THROW(enumtype.Insert(3, "error1"), std::invalid_argument);    // error
  EXPECT_THROW(enumtype.Insert(233, "faulty"), std::invalid_argument);  // error


  EXPECT_EQ(enumtype.NameToValue("closed"), 2);
  EXPECT_THROW(enumtype.NameToValue("Closed"), std::out_of_range); // error
  
  EXPECT_EQ(enumtype.ValueToName(3), "faulty");
  EXPECT_THROW(enumtype.ValueToName(4), std::out_of_range); // error
}
