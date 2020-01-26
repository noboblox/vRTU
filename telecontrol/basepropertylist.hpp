#ifndef TC_BASEPROPERTYLIST_HPP_
#define TC_BASEPROPERTYLIST_HPP_

#include <string>
#include <unordered_map>

#include "enumtype.hpp"

namespace TC
{
  class BasePropertyList
  {
  public:
    static constexpr const char* csTrue = "true";
    static constexpr const char* csFalse = "false";
    
    virtual ~BasePropertyList();

    /// pure virtual
    virtual const bool HasExplicitProperty(const std::string& arKey) const = 0;
    virtual const std::string& GetExplicitProperty(const std::string arKey) const = 0;

    virtual const bool HasDefaultProperty(const std::string& arKey) const = 0;
    virtual const std::string& GetDefaultProperty(const std::string arKey) const = 0;

    /// Non virtual
    long GetInt(const std::string& arKey, bool aAllowDefault = true) const;
    std::string GetString(const std::string arKey, bool aAllowDefault = true) const;
    double GetReal(const std::string& arKey, bool aAllowDefault = true) const;
    bool GetBool(const std::string& arKey, bool aAllowDefault = true);

    template<typename PlainEnumT>
    UTIL::Enum<PlainEnumT> GetEnum(const std::string& arKey, bool aAllowDefault = true) const
    {
      return UTIL::Enum<PlainEnumT> (GetString(arKey, aAllowDefault));
    }
  };

  class BaseDataPropertyList : public BasePropertyList
  {
    using PropertyMap = std::unordered_map<std::string, std::string>;

  public:
    static constexpr const char* csValue = "Value";
    static constexpr const char* csTimestamp = "Timestamp";
    static constexpr const char* csQuality = "Quality";

  };

}
#endif // TC_BASEPROPERTYLIST_HPP_