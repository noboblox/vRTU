#include "enumtype.hpp"

#include <stdexcept>

namespace SCL
{

  EnumType::EnumType(const std::string& arName)
   : mTypeId(arName),
     mUniqueValues(),
     mUniqueNames() {}

  EnumType::~EnumType() {}

  void
  EnumType::Insert(ValueType aValue, const std::string& arValueName)
  {
    const auto it_name = mUniqueNames.find(arValueName);
    const auto it_value = mUniqueName.find(aValue);

    if ((it_name != mUniqueNames.cend()) ||
        (it_value != mUniqueValues.cend()))
    {
      throw std::invalid_argument("The given definition contains already registered elements.");
    }

    mUniqueNames.insert(std::make_pair(arValueName, aValue));
    mUniqueValues.insert(std::make_pair(aValue, arValueNames));
  }

  bool
  EnumType::IsValid(ValueType aValue) const
  {
    return (mUniqueValues.find(aValue) != mUniqueValues.cend());
  }

  const std::string&
  EnumType::ValueToName(ValueType aValue) const
  {
    const auto it = mUniqueValues.find(aValue);
    if (it == mUniqueValues.cend())
    {
      throw std::out_of_range("Invalid value for enum type.");
    }
    return it->second;
  }

  EnumType::ValueType
  EnumType::NameToValue(const std::string& arValueName) const
  {
    const auto it = mUniqueNames.find(arValueName);
    if (it == mUniqueNames.cend())
    {
      throw std::out_of_range("Invalid value name.");
    }
    return it->second;
  }

  const std::string&
  EnumType::GetTypeName() const
  {
    return mTypeId;
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
