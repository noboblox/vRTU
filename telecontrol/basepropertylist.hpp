#ifndef TC_BASEPROPERTYLIST_HPP_
#define TC_BASEPROPERTYLIST_HPP_

#include <string>

namespace TC
{
  class BasePropertyList
  {
  public:
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

  protected:
    static constexpr const char* csTrue = "true";
    static constexpr const char* csFalse = "false";
  };
}
#endif // TC_BASEPROPERTYLIST_HPP_