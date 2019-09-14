/*
 * iecdataelement.hpp
 *
 *  Created on: 14.09.2019
 *      Author: Rene Buettgen
 */

#ifndef SCL_INCLUDE_IECDATAELEMENT_HPP_
#define SCL_INCLUDE_IECDATAELEMENT_HPP_

#include <memory>
#include <unordered_map>
#include <string>

namespace SCL
{
  class ObjectReference;

  /**
   * @brief API class for accessing IEC data (LN, DO, SDO, DA, BDA) inside a tree structure
   */
  class IecDataElement
  {
  public:
    explicit IecDataElement() // Root element
      : mpParent(this),
        mpChilds() {}
    virtual ~IecDataElement();

  public: // no copy no move
    IecDataElement(const IecDataElement&) = delete;
    IecDataElement(IecDataElement&&) = delete;
    IecDataElement& operator=(const IecDataElement&) = delete;
    IecDataElement& operator=(IecDataElement&&) = delete;

   // Access element inside the object tree
  public:
    IecDataElement& Find(const ObjectReference& arReference);
    const IecDataElement& Find(const ObjectReference& arReference) const;

    IecDataElement& GetRoot();
    const IecDataElement& GetRoot() const;

    IecDataElement& GetParent();
    const IecDataElement& GetParent() const;

    IecDataElement& GetChild(const std::string& arName);
    const IecDataElement& GetChild(const std::string& arName) const;

    // Position information about the current element
  public:
    size_t GetLayer() const;
    size_t IsRoot() const;

  private:
    const IecDataElement& TrackRootInternal(size_t* arCounter = nullptr, ObjectReference* apReference = nullptr) const;
    const IecDataElement& TrackChildInteral(const ObjectReference& arReference) const;
    using ChildPtr = std::unique_ptr<IecDataElement>;
    IecDataElement* mpParent;
    std::unordered_map<std::string, ChildPtr> mpChilds;
  };

} /* namespace SCL */

#endif /* SCL_INCLUDE_IECDATAELEMENT_HPP_ */
