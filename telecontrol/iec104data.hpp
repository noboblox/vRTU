#ifndef IEC104DATA_HPP_
#define IEC104DATA_HPP_

#include <string>
#include <memory>
#include <functional>
#include <algorithm>

#include "iec104properties.hpp"

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

    virtual bool Update(const TC::BaseDataPropertyList& arUpdate)
    {
      bool updated = false;
      if (arUpdate.HasExplicitProperty(TC::BaseDataPropertyList::csValue))
        updated |= UpdateValue(arUpdate.GetString(TC::BaseDataPropertyList::csValue));
      if (arUpdate.HasExplicitProperty(TC::BaseDataPropertyList::csTimestamp))
        updated |= UpdateTimestamp(arUpdate.GetString(TC::BaseDataPropertyList::csTimestamp));
      if (arUpdate.HasExplicitProperty(TC::BaseDataPropertyList::csQuality))
        updated |= UpdateQuality(arUpdate.GetString(TC::BaseDataPropertyList::csQuality));

      return updated;
    }

    virtual bool UpdateValue(const std::string& arUpdate) = 0;
    virtual bool UpdateTimestamp(const std::string& arUpdate) {return false;}
    virtual bool UpdateQuality(const std::string& arUpdate) {return false;}

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

  template <typename PtrT>
  class SpInfoObjectPointer
  {
  public:
    explicit SpInfoObjectPointer(PtrT apOwned)
      : mpHandle(apOwned) {}

    SpInfoObjectPointer(const SpInfoObjectPointer&) = delete;
    SpInfoObjectPointer& operator=(const SpInfoObjectPointer&) = delete;

    SpInfoObjectPointer(SpInfoObjectPointer&& arOther)
    {
      Reset(nullptr);
      mpHandle = arOther.Release();
    }

    SpInfoObjectPointer operator=(SpInfoObjectPointer&& arOther)
    {
      Reset(nullptr);
      mpHandle = arOther.Release();
    }

    void Reset(PtrT apNewOwned)
    {
      if (mpHandle)
      {
        InformationObject pBase = reinterpret_cast<InformationObject>(mpHandle);
        InformationObject_destroy(pBase);
      }
      mpHandle = apNewOwned;
    }

    PtrT Get() const
    {
      return mpHandle;
    }

    PtrT Release()
    {
      PtrT pUnowned = mpHandle;
      mpHandle = nullptr;
      return pUnowned;
    }

    ~SpInfoObjectPointer()
    {
      Reset(nullptr);
    }

  private:
      PtrT mpHandle;
  };
    
  class DataFactory
  {
  public:
    static SpDataPtr Create(const TC::BasePropertyList& arDefinition, size_t aIoaBytes = 3);
    static void VerifyRange(size_t aId, size_t aMaxBytes);
  };
   
  using EnumPosition = UTIL::Enum<DoublePointValue>;
  class DoublePointStatus : public BaseData
  {
  public:
    
    DoublePointStatus(size_t arId)
      : BaseData(std::to_string(arId), TypeIdEnum(M_DP_NA_1).GetString()),
        mpHandle(DoublePointInformation_create(nullptr, arId, IEC60870_DOUBLE_POINT_INDETERMINATE, IEC60870_QUALITY_INVALID))
    {      
    }

    virtual ~DoublePointStatus()
    {
    }
    
    virtual bool UpdateValue(const std::string& arUpdate) override
    {
      EnumPosition newValue(arUpdate);
      EnumPosition oldValue(DoublePointInformation_getValue(mpHandle.Get()));
  
      if (oldValue != newValue)
      {
        const int myId = std::stoi(GetId());
        const QualityDescriptor oldQuality = DoublePointInformation_getQuality(mpHandle.Get());
        DoublePointInformation_create(mpHandle.Get(), myId, newValue.GetValue(), oldQuality); // TODO: Quality
      }
      return false;
    }

    virtual bool UpdateTimestamp(const std::string& arUpdate)
    {
      return false; // TODO
    }

    virtual bool UpdateQuality(const std::string& arUpdate)
    {
      return false; // TODO
    }

    SpInfoObjectPointer<DoublePointInformation> mpHandle;
  };

}
#endif // !IEC104DATA_HPP_
