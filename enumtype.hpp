#ifndef ENUMTYPE_HPP
#define ENUMTYPE_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <utility>


namespace SCL
{

  class EnumType
  {
  public:
    using ValueType = int_fast32_t;
    using ValueDefinition = std::vector<std::pair<ValueType, std::string>>;

  public:
    explicit EnumType(const std::string& arName);
    virtual ~EnumType();
    void Insert(ValueType aValue, const std::string& arValueName);
    bool IsValid(ValueType aValue) const;
    const std::string& ToName(ValueType aValue) const;
    ValueType ToValue(const std::string& arValueName) const;
    const std::string& GetTypeName() const;

  private:
    std::string mId;
    ValueDefinition mValues;

  };
d
  class EnumValue
  {
  public:
    explicit EnumValue(const std::string& arValue, const EnumType& arEnumType);
    EnumValue(const EnumValue& arOther) = delete;
    EnumValue(EnumValue&& arOther) = delete;
    EnumValue& operator=(const EnumValue& arOther) = delete;
    EnumValue& operator=(EnumValue&& arOther) = delete;
    EnumValue& operator=(const std::string& arValueName);

  public:
    explicit operator EnumType::ValueType() const {return mValue;}
    const std::string& GetName() const;
    bool  IsValid() const;
    const std::string& GetType() const;
    EnumType::ValueType GetValue() const;
    bool operator==(const std::string& aRhs) const;

    inline bool operator!=(const std::string& aRhs) const {return (!this->operator==(aRhs));}

  private:
    const EnumType& mrDefinition;
    EnumType::ValueType mValue;
  };

}
#endif // ENUMTYPE_HPP
