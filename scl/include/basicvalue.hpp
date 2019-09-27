/*
 * basicvalue.hpp
 *
 *  Created on: 27.09.2019
 *      Author: Rene
 */

#ifndef SCL_INCLUDE_BASICVALUE_HPP_
#define SCL_INCLUDE_BASICVALUE_HPP_

namespace SCL
{

  class BasicValueBase
  {
  public:
    BasicValueBase() = delete;
    virtual
    ~BasicValueBase();
    BasicValueBase(const BasicValueBase &other) = delete;
    BasicValueBase(BasicValueBase &&other) = delete;
    BasicValueBase&
    operator=(const BasicValueBase &other) = delete;
    BasicValueBase&
    operator=(BasicValueBase &&other) = delete;

    template <class ValueType>
    virtual ValueType& GetValueType();
  };

} /* namespace SCL */

#endif /* SCL_INCLUDE_BASICVALUE_HPP_ */
