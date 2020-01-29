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
      // TODO
    case M_DP_NA_1:
      return SpDataPtr(new IEC104::DoublePointStatus(dataId));
    case M_DP_TA_1:
      return SpDataPtr(new IEC104::DoublePointStatusTime24(dataId));

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

  DoublePointStatus::DoublePointStatus(int arId)
    : BaseData(std::to_string(arId), TypeIdEnum(M_DP_NA_1).GetString()),
      mValue(IEC60870_DOUBLE_POINT_INDETERMINATE),
      mQuality() {}

  DoublePointStatus::DoublePointStatus(int arId, const std::string& arType)
    : BaseData(std::to_string(arId), arType),
      mValue(IEC60870_DOUBLE_POINT_INDETERMINATE),
      mQuality() {}

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

  bool
  DoublePointStatus::UpdateQuality(const std::string& arUpdate)
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
  DoublePointStatus::GetValue() const
  {
    return mValue.GetString();
  }

  std::string
  DoublePointStatus::GetQuality() const
  {
    return mQuality.GetString();
  }

  SpBaseInformation
  DoublePointStatus::Write() const
  {
    const int ioa = std::stoi(GetId());
    SpBaseInformation pNew(reinterpret_cast<InformationObject>(DoublePointInformation_create(nullptr, ioa, mValue.GetValue(), mQuality.GetInt())));
    AssertTypeIdIntegrity(pNew.Get());
    return std::move(pNew);
  }

  DoublePointStatusTime24::DoublePointStatusTime24(int arId)
    : DoublePointStatus(arId, IEC104::TypeIdEnum(M_DP_TA_1).GetString())
  {
    sCP24Time2a empty{}; // TODO -> Member
  }

  bool
  DoublePointStatusTime24::UpdateTimestamp(const std::string& arUpdate)
  {
    return false; // TODO 
  }

  std::string
  DoublePointStatusTime24::GetTimestamp() const
  {
    return false; // TODO
  }

  SpBaseInformation
  DoublePointStatusTime24::Write() const
  {
    return SpBaseInformation(nullptr); // TODO
  }

}
