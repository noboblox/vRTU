#include "iec104data.hpp"

#include <climits>

#include "iec104properties.hpp"
#include "iec104quality.hpp"

namespace IEC104
{
  SpDataPtr
  DataFactory::Create(const TC::BasePropertyList& arDefinition) const
  {
    const Iec104DataDefinition& rDefinition = dynamic_cast<const Iec104DataDefinition&> (arDefinition);
    const int dataId(rDefinition.GetInt(Iec104DataDefinition::csDataId));
    const TypeIdEnum typeId(rDefinition.GetString(Iec104DataDefinition::csTypeId));
    AssertRange(dataId, mIoaBytes);

    SpDataPtr pNew = nullptr;
    switch (typeId.GetValue())
    {
    case M_DP_NA_1:
      // TODO
      return SpDataPtr(new IEC104::DoublePointStatus(dataId));
    case M_DP_TB_1:
      return SpDataPtr(new IEC104::DoublePointStatusTime(dataId));
    case M_SP_NA_1:
      return SpDataPtr(new IEC104::SinglePointStatus(dataId));
    case M_SP_TB_1:
      return SpDataPtr(new IEC104::SinglePointStatusTime(dataId));
    case M_ME_NC_1:
      return SpDataPtr(new IEC104::FloatValue(dataId));
    case M_ME_TF_1:
      return SpDataPtr(new IEC104::FloatValueTime(dataId));

    default:
      throw std::invalid_argument("Creation failed: Unknown type");
    }
  }

  void
  DataFactory::AssertRange(size_t aTestedNumber, size_t aMaxBytes) const
  {
    if (aMaxBytes < sizeof(size_t))
    {
      const size_t availableBits = CHAR_BIT * aMaxBytes;
      const size_t ioaLimit = (size_t(1) << availableBits) - 1; // 2^n -1

      if (aTestedNumber > ioaLimit)
        throw std::invalid_argument("The information object address is too large for the server");
    }
  }

  BaseData::BaseData(const std::string& arId, const std::string& arType)
    : mId(arId), mType(arType)
  {}

  BaseData::BaseData()
    : mId("invalid"), mType("invalid") {}

  BaseData::~BaseData()
  {}

  void
  BaseData::AssertTypeIdIntegrity(InformationObject apCheckedObject) const
  {
    TypeIdEnum typeId(InformationObject_getType(apCheckedObject));
    if (typeId.GetString() != GetTypeString())
    {
      throw std::runtime_error("Critical error: IEC 104 type id mismatch. Stored type differs from the type used in external communication.");
    }
  }

  bool
  BaseData::Update(const TC::BaseDataPropertyList& arUpdate)
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

  bool
  DoublePointStatus::UpdateValue(const std::string& arUpdate)
  {
    EnumPosition newValue(arUpdate);
    if (mValue != newValue)
    {
      mValue = newValue;
      return true;
    }
    return false;
  }

  std::string
  DoublePointStatus::GetValue() const
  {
    return mValue.GetString();
  }

  bool
  DataWithQuality::UpdateQuality(const std::string& arUpdate)
  {
    Quality newQuality(arUpdate);
    if (mQuality != newQuality)
    {
      mQuality = newQuality;
      return true;
    }
    return false;
  }
  
  std::string
  DataWithQuality::GetQuality() const
  {
    return mQuality.GetString();
  }

  DataWithQuality::~DataWithQuality() {}
  DataWithTimestamp::~DataWithTimestamp() {}

  bool
  DataWithTimestamp::UpdateTimestamp(const std::string& arUpdate)
  {
    Timestamp newTimestamp(arUpdate);

    if (mTime != newTimestamp)
    {
      mTime = newTimestamp;
      return true;
    }
    return false;
  }

  std::string
  DataWithTimestamp::GetTimestamp() const
  {
    return mTime.GetString();
  }

  /// Ctors
  DoublePointStatus::DoublePointStatus(int arId)
    : BaseData(std::to_string(arId), TypeIdEnum(M_DP_NA_1).GetString()),
      mValue(IEC60870_DOUBLE_POINT_INDETERMINATE) {}

  DoublePointStatus::DoublePointStatus()
    : BaseData(),
      mValue(IEC60870_DOUBLE_POINT_INDETERMINATE) {}

  DoublePointStatusTime::DoublePointStatusTime(int arId)
    : BaseData(std::to_string(arId), TypeIdEnum(M_DP_TB_1).GetString()) {}

  /// Write(). Each data has its own write function responsible for encoding into an lib60870 InformationObject
  SpBaseInformation
  DoublePointStatus::Write() const
  {
    SpBaseInformation pNew(reinterpret_cast<InformationObject>(DoublePointInformation_create(nullptr, std::stoi(GetId()), mValue.GetValue(), mQuality.GetInt())));
    AssertTypeIdIntegrity(pNew.Get());
    return std::move(pNew);
  }

  SpBaseInformation
  DoublePointStatusTime::Write() const
  {
    sCP56Time2a time(mTime.Encode56());
    SpBaseInformation pNew(reinterpret_cast<InformationObject>(DoublePointWithCP56Time2a_create(nullptr, std::stoi(GetId()), mValue.GetValue(), mQuality.GetInt(), &time)));
    AssertTypeIdIntegrity(pNew.Get());
    return std::move(pNew);
  }


  /// Single point status

  SinglePointStatus::SinglePointStatus(int arId)
    : BaseData(std::to_string(arId), TypeIdEnum(M_SP_NA_1).GetString()),
    mValue(false) {}

  SinglePointStatus::SinglePointStatus()
    : BaseData(),
    mValue(false) {}

  bool
  SinglePointStatus::UpdateValue(const std::string& arUpdate)
  {
    bool newValue(false);
    if (arUpdate == sTrue)
      newValue = true;
    else if (arUpdate != sFalse)
      throw std::invalid_argument("Could not derive any value from the provided string.");
    
    if (mValue != newValue)
    {
      mValue = newValue;
      return true;
    }
    return false;
  }

  std::string
  SinglePointStatus::GetValue() const
  {
    return mValue ? sTrue : sFalse;
  }

  SpBaseInformation
  SinglePointStatus::Write() const
  {
    SpBaseInformation pNew(reinterpret_cast<InformationObject>(SinglePointInformation_create(nullptr, std::stoi(GetId()), mValue, mQuality.GetInt())));
    AssertTypeIdIntegrity(pNew.Get());
    return std::move(pNew);
  }

  SinglePointStatusTime::SinglePointStatusTime(int arId)
    : BaseData(std::to_string(arId), TypeIdEnum(M_SP_TB_1).GetString()) {}
  
  SpBaseInformation
  SinglePointStatusTime::Write() const
  {
    sCP56Time2a time(mTime.Encode56());
    SpBaseInformation pNew(reinterpret_cast<InformationObject>(SinglePointWithCP56Time2a_create(nullptr, std::stoi(GetId()), mValue, mQuality.GetInt(), &time)));
    AssertTypeIdIntegrity(pNew.Get());
    return std::move(pNew);
  }  
  
  /// Short Floating point  

  FloatValue::FloatValue(int arId)
    : BaseData(std::to_string(arId), TypeIdEnum(M_ME_NC_1).GetString()),
    mValue(0.0) {}

  FloatValue::FloatValue()
    : BaseData(),
    mValue(0.0) {}

  bool
  FloatValue::UpdateValue(const std::string& arUpdate)
  {
    float newValue(std::stof(arUpdate));
    if (mValue != newValue)
    {
      mValue = newValue;
      return true;
    }
    return false;
  }

  std::string
  FloatValue::GetValue() const
  {
    return std::to_string(mValue);
  }

  SpBaseInformation
  FloatValue::Write() const
  {
    SpBaseInformation pNew(reinterpret_cast<InformationObject>(MeasuredValueShort_create(nullptr, std::stoi(GetId()), mValue, mQuality.GetInt())));
    AssertTypeIdIntegrity(pNew.Get());
    return std::move(pNew);
  }

  FloatValueTime::FloatValueTime(int arId)
    : BaseData(std::to_string(arId), TypeIdEnum(M_ME_TF_1).GetString()) {}
  
  SpBaseInformation
  FloatValueTime::Write() const
  {
    sCP56Time2a time(mTime.Encode56());
    SpBaseInformation pNew(reinterpret_cast<InformationObject>(MeasuredValueShortWithCP56Time2a_create(nullptr, std::stoi(GetId()), mValue, mQuality.GetInt(), &time)));
    AssertTypeIdIntegrity(pNew.Get());
    return std::move(pNew);
  }
}
