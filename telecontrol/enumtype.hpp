#ifndef UTIL_ENUMTYPE_HPP_
#define UTIL_ENUMTYPE_HPP_

#include <stdexcept>
#include <string>
#include <utility>

namespace UTIL
{
  template <typename PlainEnumT>
  class Enum
  {
  public:
    using EnumDefinition = std::vector< std::pair<std::string, PlainEnumT> >;

    explicit Enum(const std::string& arValueString)
      : mValue(Enum<PlainEnumT>::Convert(arValueString)) {}
 
    explicit Enum(const PlainEnumT arValueEnum)
      : mValue(Convert(Convert(arValueEnum))) {} // Verify validity by double conversion

    Enum<PlainEnumT>& operator=(const std::string& arOther)
    {
      PlainEnumT targetValue = Convert(arOther);
      mValue = targetValue;
      return *this;
    }
    
    Enum<PlainEnumT>& operator=(PlainEnumT aOther)
    {
      PlainEnumT targetValue = Convert(Convert(aOther));
      mValue = targetValue;
      return *this;
    }
    
    PlainEnumT GetValue() const
    {
      return mValue;
    }

    std::string GetString() const
    {
      return Enum<PlainEnumT>::Convert(mValue);
    }

    const bool operator==(const Enum<PlainEnumT>& arOther) const {return mValue == arOther.mValue;}
    const bool operator!=(const Enum<PlainEnumT>& arOther) const { return !(this->operator==(arOther));}
    const bool operator<(const Enum<PlainEnumT>& arOther)  const {return mValue < arOther.mValue;}
    const bool operator<=(const Enum<PlainEnumT>& arOther) const { return  (this->operator< (arOther)) || (this->operator==(arOther)); }
    const bool operator>(const Enum<PlainEnumT>& arOther)  const { return !(this->operator<=(arOther));}
    const bool operator>=(const Enum<PlainEnumT>& arOther) const { return !(this->operator< (arOther) );}

  private:
    PlainEnumT mValue;

    static PlainEnumT Convert(const std::string& arValue)
    {
      for (const auto& entry : msDefinition)
      {
        if (entry.first == arValue)
          return entry.second;
      }
      throw std::invalid_argument("Provided value is no valid enum entry.");
    }

    static std::string Convert(PlainEnumT arValue)
    {
      for (const auto& entry : msDefinition)
      {
        if (entry.second == arValue)
          return entry.first;
      }
      throw std::invalid_argument("Provided value is no valid enum entry.");
    }
    static const EnumDefinition msDefinition;
  };
}

#endif