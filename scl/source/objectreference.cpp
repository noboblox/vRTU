/*
 * objectreference.cpp
 *
 *  Created on: 14.09.2019
 *      Author: Rene Buettgen
 */

#include "objectreference.hpp"

namespace SCL
{

  ObjectReference::ObjectReference(const std::string& arReference, FunctionalConstraint aFC, char arSeperator)
    : mSeperator(arSeperator),
      mFC(aFC),
      mElements()
  {
    size_t begin = 0;
    size_t end = 0;

    while (end != std::string::npos)
    {
      end = arReference.find_first_of(mSeperator, begin);
      mElements.push_back(arReference.substr(begin, (end - begin)));
      
	  begin = end + 1;
    }
  }

  ObjectReference::~ObjectReference()
  {
  }

  size_t
  ObjectReference::GetLayer() const
  {
    return mElements.size();
  }

  const std::string&
  ObjectReference::GetElementName(size_t aLayer) const
  {
    return mElements.at(aLayer - 1);
  }

} /* namespace SCL */
