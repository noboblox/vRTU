#ifndef IEC104DATA_HPP_
#define IEC104DATA_HPP_

#include <string>
#include <memory>
#include <functional>
#include <algorithm>

#include "iec104properties.hpp"
#include "iec104smartpointer.hpp"
#include "staticenums.hpp"
#include "iec104quality.hpp"

extern "C"
{
#include "iec60870_common.h"
#include "cs101_information_objects.h"
}

namespace TC
{
  class BasePropertyList;
  class BaseDataPropertyList;
}

namespace IEC104
{
  class BaseData
  {
  public:
    virtual ~BaseData();
    const std::string GetId() const {return mId;}
    const std::string GetTypeString() const {return mType;}

    virtual bool Update(const TC::BaseDataPropertyList& arUpdate);

    virtual bool UpdateValue(const std::string& arUpdate) = 0;
    virtual bool UpdateTimestamp(const std::string& arUpdate) {return false;}
    virtual bool UpdateQuality(const std::string& arUpdate) {return false;}

    // TODO: virtual TC::BaseDataPropertyList GetData() const = 0;
    virtual std::string GetValue() const = 0;
    virtual std::string GetQuality() const { return ""; }
    virtual std::string GetTimestamp() const   { return ""; }

    BaseData(const BaseData& arOther) = delete;
    BaseData(BaseData&& arOther) = delete;
    BaseData& operator=(const BaseData& arOther) = delete;
    BaseData& operator=(BaseData&& arOther) = delete;

  protected:
    explicit BaseData(const std::string& arId, const std::string& arType);

  private:
    std::string mId;
    std::string mType;

    friend class DataFactory;
  };
  

  using SpDataPtr = std::shared_ptr<BaseData>;
  class DataFactory
  {
  public:
    explicit DataFactory(size_t aIoaBytes) : mIoaBytes(aIoaBytes) {}

    SpDataPtr Create(const TC::BasePropertyList& arDefinition) const;

  private:
    void AssertRange(size_t aTestedNumber, size_t aMaxBytes) const;
    size_t mIoaBytes;
  };
    

  class DoublePointStatus : public BaseData
  {
  public:
    using SpDoublePointBase = SpInfoObjectPointer<DoublePointInformation>;

    DoublePointStatus(int arId);
    virtual ~DoublePointStatus() {}

    virtual bool UpdateValue(const std::string& arUpdate) override;
    virtual bool UpdateQuality(const std::string& arUpdate) override;

    virtual std::string GetValue() const override;

  protected:
    DoublePointStatus(int arId, const std::string& arType); // Constructor for child classes
    SpDoublePointBase mpHandle; //!< Emulated base pointer as per lib60870 inheritance support
  };


  class DoublePointStatusTime24 : public DoublePointStatus
  {
  public:
    DoublePointStatusTime24(int arId);

    virtual bool UpdateTimestamp(const std::string& arUpdate) override;
  };

}
#endif // !IEC104DATA_HPP_