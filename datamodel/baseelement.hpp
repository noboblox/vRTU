/*
 * baseelement.hpp
 *
 *  Created on: 21.12.2019
 *      Author: rene
 */

#ifndef BASEELEMENT_HPP_
#define BASEELEMENT_HPP_

#include <string>

namespace DM
{
  class Name
  {
  public:
    explicit Name(const std::string& arName)
      : mName(arName) {}

    const std::string& GetName() const {return mName;}

  private:
    std::string mName;
  };


  class BaseElement : virtual public Name
  {
  public:
    explicit BaseElement(const std::string& arName);
    virtual ~BaseElement();



    BaseElement(BaseElement&&) = delete;
    BaseElement& operator=(const BaseElement&) = delete;
    BaseElement& operator=(BaseElement&&) = delete;
    BaseElement(const BaseElement&) = delete;
  };

} /* namespace DM */

#endif /* BASEELEMENT_HPP_ */
