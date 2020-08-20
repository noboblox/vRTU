/*
 * baserequest.hpp
 *
 *  Created on: Aug 20, 2020
 *      Author: blox
 */

#ifndef BASEREQUEST_HPP_
#define BASEREQUEST_HPP_

namespace TC
{
  namespace REST
  {

    class BaseRequest
    {
    public:
      BaseRequest();
      virtual ~BaseRequest();
      BaseRequest(const BaseRequest &other) = delete;
      BaseRequest(BaseRequest &&other) = delete;
      BaseRequest& operator=(const BaseRequest &other) = delete;
      BaseRequest& operator=(BaseRequest &&other) = delete;
    };

  } /* namespace REST */
} /* namespace TC */

#endif /* BASEREQUEST_HPP_ */
