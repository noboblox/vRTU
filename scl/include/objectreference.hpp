/*
 * objectreference.hpp
 *
 *  Created on: 14.09.2019
 *      Author: Rene Buettgen
 */

#ifndef SCL_INCLUDE_OBJECTREFERENCE_HPP_
#define SCL_INCLUDE_OBJECTREFERENCE_HPP_

#include <string>
#include <vector>

#include "enums.hpp"

namespace SCL
{

  class ObjectReference
  {
  public:
    explicit ObjectReference(const std::string& arReference, FunctionalConstraint aFC = FunctionalConstraint::undefined, char arSeperator = '.');
    virtual ~ObjectReference();

  public:
    size_t GetLayer() const;
    const std::string& GetElementName(size_t aLayer) const;

  public: // TODO: object references are copyable
    ObjectReference(const ObjectReference &other) = delete;
    ObjectReference(ObjectReference &&other) = delete;
    ObjectReference& operator=(const ObjectReference &other) = delete;
    ObjectReference& operator=(ObjectReference &&other) = delete;

  private:
    char mSeperator;
    FunctionalConstraint mFC;
    std::vector<std::string> mElements;
  };

} /* namespace SCL */

#endif /* SCL_INCLUDE_OBJECTREFERENCE_HPP_ */
