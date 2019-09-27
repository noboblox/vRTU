/*
 * datatemplatecontainer.hpp
 *
 *  Created on: 27.09.2019
 *      Author: Rene Buettgen
 */

#ifndef SCL_INCLUDE_DATATEMPLATECONTAINER_HPP_
#define SCL_INCLUDE_DATATEMPLATECONTAINER_HPP_

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace SCL
{
  class EnumType;
  class DataAttributeType;
  class DataObjectType;

  class DataTemplateContainer
  {
  public:
    template <class DataType>
    using DataPtr = std::unique_ptr<DataType>;
    template <class DataType>
    using DataContainer = std::vector<DataPtr<DataType>>;

  public:
    explicit DataTemplateContainer();
    virtual ~DataTemplateContainer();

  public: // no copy no move
    DataTemplateContainer(const DataTemplateContainer&) = delete;
    DataTemplateContainer(DataTemplateContainer&&) = delete;
    DataTemplateContainer& operator=(const DataTemplateContainer&) = delete;
    DataTemplateContainer& operator=(DataTemplateContainer&&) = delete;

    /**
     * @brief Adds a type into the container, managing its memory. The passed pointer will be set to nullptr.
     * @param apManagedObject Raw-pointer with the managed object
     */
    EnumType& Manage(DataPtr<EnumType>&& apManagedObj);
    DataAttributeType& Manage(DataPtr<DataAttributeType>&& apManagedObj);
    DataObjectType& Manage(DataPtr<DataObjectType>&& apManagedObj);

    /**
     * @brief Destroys all held data.
     */
    void Clear();

    /**
     * @brief Gets a specific type by name.
     * @param arName Name of the type
     * @return Reference to the found type
     *
     * @attention Throws out_of_range if the type is not found.
     */
    const EnumType& GetEnumType(const std::string& arName) const;
    const DataAttributeType& GetDAType(const std::string& arName) const;
    const DataObjectType& GetDOType(const std::string& arName) const;

  private:
    DataContainer<EnumType> mpEnumTypes;
    DataContainer<DataAttributeType> mpDATypes;
    DataContainer<DataObjectType> mpDOTypes;

    template <class DataType>
    const DataType* GetInternal(const DataContainer<DataType>& arContainer, const std::string& arName, bool aExceptionOnFail) const;

    template <class DataType>
    DataType& ManageInternal(DataContainer<DataType>& arContainer, DataPtr<DataType>&& arrObjPtr);

  };

} /* namespace SCL */

#endif /* SCL_INCLUDE_DATATEMPLATECONTAINER_HPP_ */
