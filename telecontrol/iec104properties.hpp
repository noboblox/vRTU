#ifndef _TC_IEC104PROPERTIES_HPP_
#define _TC_IEC104PROPERTIES_HPP_

#include "basepropertylist.hpp"

#include <unordered_map>
#include <string>

namespace TC
{
  class Iec104DataDefiniton : public BasePropertyList
  {
    using PropertyMap = std::unordered_map<std::string, std::string>;

  public:
    static constexpr const char* csTypeId = "TypeId"; //!< ASDU type for the data point
    static constexpr const char* csDataId = "DataId"; //!< Information object address

  public:
    Iec104DataDefiniton(long aTypeId, long aDataId);
    virtual ~Iec104DataDefiniton();

    virtual const bool HasExplicitProperty(const std::string& arKey) const override;
    virtual const std::string& GetExplicitProperty(const std::string arKey) const override;

    virtual const bool HasDefaultProperty(const std::string& arKey) const override;
    virtual const std::string& GetDefaultProperty(const std::string arKey) const override;

  protected:
    PropertyMap mProperties;
  };

}
#endif
