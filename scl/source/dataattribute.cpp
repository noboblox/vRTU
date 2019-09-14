#include "dataattribute.hpp"

namespace SCL
{
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

    DataAttribute::DataAttribute(const std::string& arName, DataAttributeBase::BasicType aBasicType, FunctionalConstraint aFc, Trigger aTrigger)
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

