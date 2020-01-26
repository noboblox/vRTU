#ifndef IEC104DATA_HPP_
#define IEC104DATA_HPP_

#include <string>
#include <memory>

namespace TC
{
  class BasePropertyList;
}

namespace IEC104
{
  
  class BaseData
  {
  public:
    virtual ~BaseData();
    const std::string GetId() const {return mId;}
    const std::string GetTypeString() const {return mType;}

    BaseData(const BaseData& arOther) = delete;
    BaseData(BaseData&& arOther) = delete;
    BaseData& operator=(const BaseData& arOther) = delete;
    BaseData& operator=(BaseData&& arOther) = delete;

  private:
    explicit BaseData(const std::string& arId, const std::string& arType);
    std::string mId;
    std::string mType;

    friend class DataFactory;
  };
  
  using SpDataPtr = std::unique_ptr<BaseData>;

  
  class DataFactory
  {
  public:
    static SpDataPtr Create(const TC::BasePropertyList& arDefinition);
  };

}
#endif // !IEC104DATA_HPP_
