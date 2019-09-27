#include "dataobject.hpp"

#include "dataattribute.hpp"

namespace SCL
{
  DataObjectBase::DataObjectBase(const std::string& arName, const DataObjectType& arType) // Struct constructor
        : mName(arName), mType(arType.GetTypeName()) {}

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
