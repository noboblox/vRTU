#include "basepropertylist.hpp"

#include <stdexcept>
#include <typeinfo>

namespace TC
{

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