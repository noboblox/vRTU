#ifndef _TC_IEC104PROPERTIES_HPP_
#define _TC_IEC104PROPERTIES_HPP_

#include "basepropertylist.hpp"

#include <unordered_map>
#include <string>

#include "staticenums.hpp"

namespace IEC104
{
  class Iec104DataDefinition : public TC::BasePropertyList
  {
  public:
    static constexpr const char* csTypeId = "TypeId"; //!< ASDU type for the data point
    static constexpr const char* csDataId = "DataId"; //!< Information object address

  public:
    Iec104DataDefinition(const TypeIdEnum& arTypeId, long aDataId);
    virtual ~Iec104DataDefinition();

    virtual const bool HasExplicitProperty(const std::string& arKey) const override;
    virtual const std::string& GetExplicitProperty(const std::string arKey) const override;

    virtual const bool HasDefaultProperty(const std::string& arKey) const override;
    virtual const std::string& GetDefaultProperty(const std::string arKey) const override;
  };


}
#endif
