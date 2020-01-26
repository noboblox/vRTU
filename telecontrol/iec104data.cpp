#include "iec104data.hpp"
#include "iec104properties.hpp"


namespace IEC104
{
  SpDataPtr
  DataFactory::Create(const TC::BasePropertyList& arDefinition)
  {
    const Iec104DataDefinition& rDefinition = dynamic_cast<const Iec104DataDefinition&> (arDefinition);

    const std::string dataId(rDefinition.GetString(Iec104DataDefinition::csDataId));
    const TypeIdEnum typeId(rDefinition.GetString(Iec104DataDefinition::csTypeId));

    return SpDataPtr(new BaseData(dataId, typeId.GetString())); // TODO: polymorphic types
  }

  BaseData::BaseData(const std::string& arId, const std::string& arType)
    : mId(arId), mType(arType)
  {}

  BaseData::~BaseData()
  {}

}