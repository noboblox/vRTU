#include "dataobject.hpp"

#include "dataattribute.hpp"

namespace SCL
{
	DataObjectBase::~DataObjectBase()
	{
	}

	void
		DataObjectType::Insert(const SubDataObject& arSDO)
	{
		mSDOs.push_back(arSDO);
	}

	void
		DataObjectType::Insert(const DataAttribute& arDA)
	{
		mDAs.push_back(arDA);
	}

}