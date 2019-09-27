/*
 * datatemplatecontainer.cpp
 *
 *  Created on: 27.09.2019
 *      Author: Rene Buettgen
 */

#include "datatemplatecontainer.hpp"

#include "dataattribute.hpp"
#include "dataobject.hpp"
#include "enumtype.hpp"

namespace SCL
{

  DataTemplateContainer::DataTemplateContainer()
    : mpEnumTypes(),
      mpDATypes(),
      mpDOTypes() {}

  DataTemplateContainer::~DataTemplateContainer()
  {
  }

  EnumType&
  DataTemplateContainer::Manage(DataPtr<EnumType>&& apManagedObj)
  {
    return ManageInternal<EnumType>(mpEnumTypes, std::move(apManagedObj));
  }

  DataAttributeType&
  DataTemplateContainer::Manage(DataPtr<DataAttributeType>&& apManagedObj)
  {
    return ManageInternal<DataAttributeType>(mpDATypes, std::move(apManagedObj));
  }

  DataObjectType&
  DataTemplateContainer::Manage(DataPtr<DataObjectType>&& apManagedObj)
  {
    return ManageInternal<DataObjectType>(mpDOTypes, std::move(apManagedObj));
  }

  void
  DataTemplateContainer::Clear()
  {
    mpEnumTypes.clear();
    mpDOTypes.clear();
    mpDATypes.clear();
  }

  const EnumType&
  DataTemplateContainer::GetEnumType(const std::string& arName) const
  {
    return *GetInternal<EnumType>(mpEnumTypes, arName, true);
  }

  const DataAttributeType&
  DataTemplateContainer::GetDAType(const std::string& arName) const
  {
    return *GetInternal<DataAttributeType>(mpDATypes, arName, true);
  }

  const DataObjectType&
  DataTemplateContainer::GetDOType(const std::string& arName) const
  {
    return *GetInternal<DataObjectType>(mpDOTypes, arName, true);
  }

  template <class DataType>
  DataType&
  DataTemplateContainer::ManageInternal(DataContainer<DataType>& arContainer, DataPtr<DataType>&& arrObjPtr)
  {
    if (!arrObjPtr)
    {
      throw std::invalid_argument("Error nullptr passed to DataTemplateContainer");
    }

    if (GetInternal<DataType>(arContainer, arrObjPtr->GetTypeName(), false) != nullptr)
    {
      std::stringstream msg;
      msg << "Error. Duplicate type \"" << arrObjPtr->GetTypeName() << "\"." << std::endl;
      throw std::invalid_argument(msg.str());
    }

    arContainer.push_back(std::move(arrObjPtr));
    return **arContainer.rbegin();

  }

  template <class DataType>
  const DataType*
  DataTemplateContainer::GetInternal(const DataContainer<DataType>& arContainer, const std::string& arName, bool aExceptionOnFail) const
  {
    for (const auto& type : arContainer)
    {
      if (type->GetTypeName() == arName)
      {
        return type.get();
      }
    }

    if (aExceptionOnFail)
    {
      std::stringstream msg;
      msg << "Error could not Get DataType for typename: \"" << arName << "\"." << std::endl;
      throw std::out_of_range(msg.str());
    }
    return nullptr;
  }



} /* namespace SCL */
