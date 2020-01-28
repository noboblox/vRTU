#include "iec104properties.hpp"

#include <stdexcept>

namespace IEC104
{
  constexpr const char* Iec104DataDefinition::csTypeId;
  constexpr const char* Iec104DataDefinition::csDataId;
   
  Iec104DataDefinition::Iec104DataDefinition(const TypeIdEnum& arTypeId, long aDataId)
    : BasePropertyList(),
      mProperties{ {csTypeId, arTypeId.GetString()},
                   {csDataId, std::to_string(aDataId)} }
  {}
  
  Iec104DataDefinition::~Iec104DataDefinition() {}


  const bool
  Iec104DataDefinition::HasExplicitProperty(const std::string& arKey) const
  {
    return mProperties.find(arKey) != mProperties.cend();
  }

  const std::string&
  Iec104DataDefinition::GetExplicitProperty(const std::string arKey) const
  {
    if (!HasExplicitProperty(arKey))
      throw std::invalid_argument("Requested property does not exist");
    return mProperties.find(arKey)->second;
  }

  const bool
  Iec104DataDefinition::HasDefaultProperty(const std::string& arKey) const
  {
    return false;
  }
  
  const std::string&
  Iec104DataDefinition::GetDefaultProperty(const std::string arKey) const
  {
    throw std::invalid_argument("Key has no default argument.");
  }
}