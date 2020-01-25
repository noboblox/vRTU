#include "iec104properties.hpp"

#include <stdexcept>

namespace TC
{

  Iec104DataDefiniton::Iec104DataDefiniton(long aTypeId, long aDataId)
    : BasePropertyList(),
      mProperties{ {csTypeId, std::to_string(aTypeId)},
                   {csDataId, std::to_string(aDataId)} }
  {}
  
  Iec104DataDefiniton::~Iec104DataDefiniton() {}


  const bool
  Iec104DataDefiniton::HasExplicitProperty(const std::string& arKey) const
  {
    return mProperties.find(arKey) != mProperties.cend();
  }

  const std::string&
  Iec104DataDefiniton::GetExplicitProperty(const std::string arKey) const
  {
    if (!HasExplicitProperty(arKey))
      throw std::invalid_argument("Requested property does not exist");
    return mProperties.find(arKey)->second;
  }

  const bool
  Iec104DataDefiniton::HasDefaultProperty(const std::string& arKey) const
  {
    return false;
  }
  
  const std::string&
  Iec104DataDefiniton::GetDefaultProperty(const std::string arKey) const
  {
    throw std::invalid_argument("Key has no default argument.");
  }
}