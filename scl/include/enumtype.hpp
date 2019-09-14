#ifndef ENUMTYPE_HPP
#define ENUMTYPE_HPP

#include <cstdint>
#include <map>
#include <string>


namespace SCL
{

  class EnumType
  {
  public:
    using ValueType = int_fast32_t;
    using ValueMap = std::map<ValueType, std::string>;
    using NameMap = std::map<std::string, ValueType>;

  public:
    explicit EnumType(const std::string& arName);
    virtual ~EnumType();
    void Insert(ValueType aValue, const std::string& arValueName);
    bool IsValid(ValueType aValue) const;
    const std::string& ValueToName(ValueType aValue) const;
    ValueType NameToValue(const std::string& arValueName) const;
    const std::string& GetTypeName() const;

  public: // Types are not copyable
	EnumType(const EnumType& arOther) = delete;
	EnumType(EnumType&& arOther) = delete;
	EnumType& operator=(const EnumType& arOther) = delete;
	EnumType& operator=(EnumType&& arOther) = delete;

  private:
    std::string mTypeId;
    ValueMap mUniqueValues;
    NameMap mUniqueNames;
  };
}
#endif // ENUMTYPE_HPP
