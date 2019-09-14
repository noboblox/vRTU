/*
 * enums.hpp
 *
 *  Created on: 15.09.2019
 *      Author: Rene
 */

#ifndef SCL_INCLUDE_ENUMS_HPP_
#define SCL_INCLUDE_ENUMS_HPP_

namespace SCL
{

  enum class FunctionalConstraint
  {
    undefined = -1,
    begin = 0,

    ST = begin,
    MX,
    CO,
    SP,
    SG,
    SE,
    SV,
    CF,
    DC,
    EX,
    SR,
    BL,
    OR,

    end
  };

}


#endif /* SCL_INCLUDE_ENUMS_HPP_ */
