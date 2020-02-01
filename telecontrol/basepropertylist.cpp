#include "basepropertylist.hpp"

#include <stdexcept>
#include <typeinfo>

namespace TC
{

  BaseDataPropertyList::BaseDataPropertyList(const std::string& arValue, const std::string& arQuality, const std::string& arTimestamp)
  {
    if (!arValue.empty())
      mProperties.insert({csValue, arValue});
    if (!arQuality.empty())
      mProperties.insert({csQuality, arQuality});
    if (!arTimestamp.empty())
      mProperties.insert({csTimestamp, arTimestamp});
  }

  const bool
  BaseDataPropertyList::HasExplicitProperty(const std::string& arKey) const
  {
    return mProperties.find(arKey) != mProperties.cend();
  }

  const std::string&
  BaseDataPropertyList::GetExplicitProperty(const std::string arKey) const
  {
    const auto resultIt = mProperties.find(arKey);
    if (resultIt == mProperties.cend())
      throw std::invalid_argument("Property not found");
    return resultIt->second;
  }

  const std::string& 
  BaseDataPropertyList::GetDefaultProperty(const std::string arKey) const
  {
    throw std::invalid_argument("No default properties present");
  }

  BasePropertyList::~BasePropertyList() {}

  long
  BasePropertyList::GetInt(const std::string& arKey, bool aAllowDefault) const
  {
    return std::stol(GetString(arKey, aAllowDefault));
  }

  std::string
  BasePropertyList::GetString(const std::string arKey, bool aAllowDefault) const
  {
    if (HasExplicitProperty(arKey))
      return GetExplicitProperty(arKey);

    if (aAllowDefault && HasDefaultProperty(arKey))
      return GetDefaultProperty(arKey);

    throw std::invalid_argument("Invalid property key.");
  }

  double
  BasePropertyList::GetReal(const std::string& arKey, bool aAllowDefault) const
  {
    return std::stod(GetString(arKey, aAllowDefault));
  }

  bool
  BasePropertyList::GetBool(const std::string& arKey, bool aAllowDefault)
  {
    const std::string boolStr = GetString(arKey, aAllowDefault);

    if (boolStr == csTrue)
      return true;
    if (boolStr == csFalse)
      return false;

    throw std::bad_cast();
  }
}