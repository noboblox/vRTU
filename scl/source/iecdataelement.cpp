/*
 * iecdataelement.cpp
 *
 *  Created on: 14.09.2019
 *      Author: Rene Buettgen
 */

#include "iecdataelement.hpp"
#include "objectreference.hpp"

namespace SCL
{

  IecDataElement::~IecDataElement()
  {
  }

  const IecDataElement&
  IecDataElement::Find(const ObjectReference& arReference) const
  {
    if (!IsRoot())
    {
      return GetRoot().Find(arReference);
    }
    else
    {
      return TrackChildInteral(arReference);
    }
  }

  const IecDataElement&
  IecDataElement::TrackChildInteral(const ObjectReference& arReference) const
  {
    size_t my_layer = GetLayer();
    if (arReference.GetLayer() == my_layer)
    {
      return *this;
    }
    else
    {
     const  ChildPtr& p_child = mpChilds.at(arReference.GetElementName(my_layer));
      return p_child->TrackChildInteral(arReference);
    }
  }

  const IecDataElement&
  IecDataElement::GetRoot() const
  {
    return TrackRootInternal();
  }

  const IecDataElement&
  IecDataElement::GetParent() const
  {
    return *mpParent;
  }

  const IecDataElement&
  IecDataElement::GetChild(const std::string& arName) const
  {
    return *mpChilds.at(arName);
  }


  IecDataElement&
  IecDataElement::Find(const ObjectReference& arReference)
  {
    const IecDataElement& found = const_cast<const IecDataElement&>(*this).Find(arReference);
    return const_cast<IecDataElement&> (found);
  }

  IecDataElement&
  IecDataElement::GetRoot()
  {
    const IecDataElement& root = const_cast<const IecDataElement&>(*this).GetRoot();
    return const_cast<IecDataElement&> (root);
  }

  IecDataElement&
  IecDataElement::GetParent()
  {
    const IecDataElement& parent = const_cast<const IecDataElement&>(*this).GetParent();
    return const_cast<IecDataElement&> (parent);
  }

  IecDataElement&
  IecDataElement::GetChild(const std::string& arName)
  {
    const IecDataElement& child = const_cast<const IecDataElement&>(*this).GetChild(arName);
    return const_cast<IecDataElement&> (child);
  }

  size_t
  IecDataElement::GetLayer() const
  {
    size_t out = 0;
    TrackRootInternal(&out);
    return out;
  }

  size_t
  IecDataElement::IsRoot() const
  {
    return mpParent == this;
  }

  const IecDataElement&
  IecDataElement::TrackRootInternal(size_t* apCounter, ObjectReference* apReference) const
  {
    if (IsRoot())
      return *this;

    if (apCounter)
      ++(*apCounter);

    return mpParent->TrackRootInternal(apCounter, apReference);
  }

} /* namespace SCL */
