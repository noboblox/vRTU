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
      mpHandle(DoublePointInformation_create(nullptr, arId, IEC60870_DOUBLE_POINT_INDETERMINATE,
                                             IEC60870_QUALITY_INVALID))
  {
  }

  DoublePointStatus::DoublePointStatus(int arId, const std::string& arType)
    : BaseData(std::to_string(arId), arType),
      mpHandle(nullptr) {}

  bool
  DoublePointStatus::UpdateValue(const std::string& arUpdate)
  {
    EnumPosition newValue(arUpdate);
    EnumPosition oldValue(DoublePointInformation_getValue(mpHandle.Get()));

    if (oldValue != newValue)
    {
      const int myId = std::stoi(GetId());
      const QualityDescriptor oldQuality = DoublePointInformation_getQuality(mpHandle.Get());
      DoublePointInformation_create(mpHandle.Get(), myId, newValue.GetValue(), oldQuality);
      return true;
    }
    return false;
  }

  bool
  DoublePointStatus::UpdateQuality(const std::string& arUpdate)
  {
    Quality newQuality(arUpdate);
    Quality oldQuality(DoublePointInformation_getQuality(mpHandle.Get()));

    if (newQuality != oldQuality)
    {
      const int myID = std::stoi(GetId());
      DoublePointValue savedValue = DoublePointInformation_getValue(mpHandle.Get());

      DoublePointInformation_create(mpHandle.Get(), myID, savedValue, newQuality.GetInt());
      return true;
    }
    return false;
  }

  std::string
  DoublePointStatus::GetValue() const
  {
    DoublePointValue value = DoublePointInformation_getValue(mpHandle.Get());
    return EnumPosition(value).GetString();
  }

  DoublePointStatusTime24::DoublePointStatusTime24(int arId)
    : DoublePointStatus(arId, IEC104::TypeIdEnum(M_DP_TA_1).GetString())
  {
    sCP24Time2a empty{};
    mpHandle = SpDoublePointBase(reinterpret_cast<DoublePointInformation>(
    DoublePointWithCP24Time2a_create(nullptr, arId, IEC60870_DOUBLE_POINT_INDETERMINATE,
                                     IEC60870_QUALITY_INVALID, &empty)));
  }

  std::string
  DoublePointStatus::GetQuality() const
  {
    Quality result(DoublePointInformation_getQuality(mpHandle.Get()));
    return result.GetString();
  }

  bool
  DoublePointStatusTime24::UpdateTimestamp(const std::string& arUpdate)
  {
    DoublePointWithCP24Time2a pHandle = reinterpret_cast<DoublePointWithCP24Time2a> (mpHandle.Get());
    //TimeStamp newTime(arUpdate);
    //TimeStamp oldTime(DoublePointWithCP24Time2a_getTimestamp(pHandle));

    sCP24Time2a newTime = { 0xFA, 0x11, 0x35 };

    //  if (newTime != oldTime)
     // {
    const int myID = std::stoi(GetId());
    DoublePointValue savedValue = DoublePointInformation_getValue(mpHandle.Get());
    QualityDescriptor savedQuality = DoublePointInformation_getQuality(mpHandle.Get());
    DoublePointWithCP24Time2a_create(pHandle, myID, savedValue, savedQuality, &newTime);
    //      }

    return false;
  }


}
