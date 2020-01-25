#ifndef _TC_IEC104PROPERTIES_HPP_
#define _TC_IEC104PROPERTIES_HPP_

#include "basepropertylist.hpp"

#include <unordered_map>
#include <string>

#include "enumtype.hpp"

extern "C"
{
#include "iec60870_common.h"
#include "cs101_information_objects.h"
}

namespace TC
{
  using TypeIdEnum = UTIL::Enum<TypeID>;
  
  class Iec104DataDefinition : public BasePropertyList
  {
    using PropertyMap = std::unordered_map<std::string, std::string>;

  public:
    static constexpr const char* csTypeId = "TypeId"; //!< ASDU type for the data point
    static constexpr const char* csDataId = "DataId"; //!< Information object address

  public:
    Iec104DataDefinition(long aTypeId, long aDataId);
    virtual ~Iec104DataDefinition();

    virtual const bool HasExplicitProperty(const std::string& arKey) const override;
    virtual const std::string& GetExplicitProperty(const std::string arKey) const override;

    virtual const bool HasDefaultProperty(const std::string& arKey) const override;
    virtual const std::string& GetDefaultProperty(const std::string arKey) const override;

  protected:
    PropertyMap mProperties;
  };

}
#endif
