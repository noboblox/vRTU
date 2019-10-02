#include "dataattribute.hpp"
#include "enumtype.hpp"

#include <sstream>
#include <stdexcept>

namespace SCL
{

  DataAttributeBase::DataAttributeBase(const std::string& arName, const EnumType& arEnum)
    : mName(arName), mBasicType(BasicTypeId::BasicType_enum), mTypeName(arEnum.GetTypeName()) {}

  DataAttributeBase::DataAttributeBase(const std::string& arName, const DataAttributeType& arStruct)
    : mName(arName), mBasicType(BasicTypeId::BasicType_struct), mTypeName(arStruct.GetTypeName()) {}

  DataAttributeBase::DataAttributeBase(const std::string& arName, const SCL::BasicTypeId& aBasicType)
    : mName(arName), mBasicType(aBasicType), mTypeName(BasicTypeId::GetString(aBasicType))
  {
    if (!mBasicType.IsValid())
    {
      std::stringstream msg;

      msg << "Cannot initialize DataAttributeBase with basic type: " << static_cast<int>(mBasicType.mType) << " = "
          << BasicTypeId::GetString(mBasicType) << "- Use dedicated constructors for enum/struct types." << std::endl;
      throw std::invalid_argument(msg.str());
    }
  }

  void
  DataAttributeType::Insert(const SubElement& arSubData)
  {
      mBDAs.push_back(arSubData);
  }

  DataAttributeBase::~DataAttributeBase()
  {
  }

  DataAttribute::DataAttribute(const std::string& arName, const EnumType& arEnum, FunctionalConstraint aFc, Trigger aTrigger)
      : DataAttributeBase(arName, arEnum),
      mFC(aFc),
      mTriggerFlags(static_cast<int>(aTrigger))
  {

  }

  DataAttribute::DataAttribute(const std::string& arName, const DataAttributeType& arStruct, FunctionalConstraint aFc, Trigger aTrigger)
      : DataAttributeBase(arName, arStruct),
      mFC(aFc),
      mTriggerFlags(static_cast<int>(aTrigger))
  {

  }

  DataAttribute::DataAttribute(const std::string& arName, const BasicTypeId& aBasicType, FunctionalConstraint aFc, Trigger aTrigger)
      : DataAttributeBase(arName, aBasicType),
      mFC(aFc),
      mTriggerFlags(static_cast<int>(aTrigger))
  {

  }

  void
  DataAttribute::CheckInit() const
  {
    if ((mFC < FunctionalConstraint::begin) || (mFC >= FunctionalConstraint::end))
    {
      throw std::invalid_argument("Error: invalid functional constraint provided to data attriubte.");
    }
  }


}

