#include "iec104data.hpp"

#include <climits>

#include "iec104properties.hpp"

template<>
IEC104::EnumPosition::EnumDefinition const IEC104::EnumPosition::msDefinition
{
  {"intermediate", IEC60870_DOUBLE_POINT_INTERMEDIATE},
  {"off", IEC60870_DOUBLE_POINT_OFF},
  {"on", IEC60870_DOUBLE_POINT_ON},
  {"faulty", IEC60870_DOUBLE_POINT_INDETERMINATE}
};

namespace IEC104
{
  SpDataPtr
    DataFactory::Create(const TC::BasePropertyList& arDefinition, size_t aIoaByte)
  {
    const Iec104DataDefinition& rDefinition = dynamic_cast<const Iec104DataDefinition&> (arDefinition);
    const int dataId(rDefinition.GetInt(Iec104DataDefinition::csDataId));
    const TypeIdEnum typeId(rDefinition.GetString(Iec104DataDefinition::csTypeId));
    
    DataFactory::VerifyRange(dataId, aIoaByte);

    SpDataPtr pNew = nullptr;

    switch (typeId.GetValue())
    {
      // TODO
    case M_DP_NA_1:
      return SpDataPtr(new IEC104::DoublePointStatus(dataId));

    default:
      throw std::invalid_argument("Creation failed: Unknown type");
    }
  }

  void
  DataFactory::VerifyRange(size_t aId, size_t aMaxBytes)
  {
    if (aMaxBytes < sizeof(size_t))
    {
      const size_t availableBits = CHAR_BIT * aMaxBytes;
      const size_t ioaLimit = (size_t(1) << availableBits) - 1; // 2^n -1

      if (aId > ioaLimit)
        throw std::invalid_argument("The information object address is too large for the server");
    }
  }

  BaseData::BaseData(const std::string& arId, const std::string& arType)
    : mId(arId), mType(arType)
  {}

  BaseData::~BaseData()
  {}

}