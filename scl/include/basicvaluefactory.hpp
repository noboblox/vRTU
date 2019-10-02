/*
 * basicvaluefactory.hpp
 *
 *  Created on: 28.09.2019
 *      Author: Rene Buettgen
 */

#ifndef SCL_INCLUDE_BASICVALUEFACTORY_HPP_
#define SCL_INCLUDE_BASICVALUEFACTORY_HPP_

#include <memory>
#include "enums.hpp"

namespace SCL
{
  class BasicValueBase;

  class BasicValueFactory
  {
  public:
    using ValuePtr = std::unique_ptr<BasicValueBase>;

  public:
    explicit BasicValueFactory() {}
    virtual ~BasicValueFactory() {}

    virtual ValuePtr Create(const BasicTypeId& aType);

  public:
    BasicValueFactory(const BasicValueFactory&) = delete;
    BasicValueFactory(BasicValueFactory&&) = delete;
    BasicValueFactory& operator=(const BasicValueFactory&) = delete;
    BasicValueFactory& operator=(BasicValueFactory&&) = delete;
  };

} /* namespace SCL */

#endif /* SCL_INCLUDE_BASICVALUEFACTORY_HPP_ */
