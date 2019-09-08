#include "enumtype.hpp"

#include <stdexcept>

namespace SCL
{

  EnumType::EnumType(const std::string& arName)
   : mId(arName),
     mValues() {}

  EnumType::~EnumType() {}

  void
  EnumType::Insert(ValueType aValue, const std::string& arValueName)
  {
    auto it_check = mValues.begin();
    auto it_end = mValues.end();
    auto it_insert = it_end;

    for (; it_check != it_end; ++it_check)
    {
      if (it_check->first >= aValue)
      {
        it_insert = it_check;
      }

      if ((it_check->first == aValue) || (it_check->second == arValueName))
      {
        throw std::invalid_argument("The element for insertion is not unique inside the enum type.");
      }
    }

    mValues.insert(it_insert, std::make_pair(aValue, arValueName));
  }

  bool
  EnumType::IsValid(ValueType aValue) const
  {
    for (const auto& i : mValues)
    {
      if (i.first == aValue)
      {
        return true;
      }
    }
    return false;
  }

  const std::string&
  EnumType::ToName(ValueType aValue) const
  {
    for (const auto& i: mValues)
    {
      if (i.first == aValue)
      {
        return i.second;
      }
    }

    throw std::out_of_range("Invalid value for enum type.");
  }

  EnumType::ValueType
  EnumType::ToValue(const std::string& arValueName) const
  {
    for (const auto& i: mValues)
    {
      if (i.second == arValueName)
      {
        return i.first;
      }
    }

    throw std::out_of_range("Invalid value name.");
  }

  const std::string&
  EnumType::GetTypeName() const
  {
    return mId;
  }

  EnumValue::EnumValue(const std::string& arValue, const EnumType& arEnumType)
   : mrDefinition(arEnumType),
     mValue(mrDefinition.ToValue(arValue)) {}

  EnumValue&
  EnumValue::operator=(const std::string& arValue)
  {
    mValue = mrDefinition.ToValue(arValue);
    return *this;
  }


  const std::string&
  EnumValue::GetName() const
  {
    return mrDefinition.ToName(mValue);
  }

  const std::string&
  EnumValue::GetType() const
  {
    return mrDefinition.GetTypeName();
  }

  EnumType::ValueType
  EnumValue::GetValue() const
  {
    return mValue;
  }

  bool
  EnumValue::operator==(const std::string& aRhs) const
  {
    return (mValue == mrDefinition.ToValue(aRhs));
  }

  bool
  EnumValue::IsValid() const
  {
    return mrDefinition.IsValid(mValue);
  }
}
