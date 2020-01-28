#ifndef STATIC_ENUMS_HPP_
#define STATIC_ENUMS_HPP_

#include "enumtype.hpp"

extern "C"
{
#include "iec60870_common.h"
#include "cs101_information_objects.h"
}

namespace IEC104
{
  using TypeIdEnum = UTIL::Enum<TypeID>;

  using EnumPosition = UTIL::Enum<DoublePointValue>; 
}

#endif // !STATIC_ENUMS_HPP_


