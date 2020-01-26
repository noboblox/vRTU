/*
 * serverparameter.hpp
 *
 *  Created on: 16.12.2019
 *      Author: rene
 */

#ifndef SERVERPARAMETER_HPP_
#define SERVERPARAMETER_HPP_

#include <boost/asio/ip/address.hpp>

namespace TC
{
  struct BaseServerParameter
  {
    BaseServerParameter(const boost::asio::ip::address& aIp, uint16_t aPort)
      : mIpAddress(aIp), mPort(aPort) {}

    virtual ~BaseServerParameter() {}

  public:
    boost::asio::ip::address mIpAddress;
    uint16_t mPort;
  };
}

namespace IEC104
{
  struct Iec104ServerParameter : public TC::BaseServerParameter
  {
   Iec104ServerParameter(const boost::asio::ip::address& aIp, uint16_t aPort, uint16_t aServerId)
      : TC::BaseServerParameter(aIp, aPort),
        mId(aServerId) {}

   virtual ~Iec104ServerParameter() {}

  public:
    static Iec104ServerParameter LocalDefaultServer(uint16_t aServerId)
    {
      return Iec104ServerParameter(boost::asio::ip::address::from_string("127.0.0.1"), msDefaultServerPort, aServerId);
    }

    static constexpr uint16_t msDefaultServerPort = 2404;

  public:
    uint16_t mId; //!< IEC 104 common address for which IOAs the server will respond
  };
}

#endif /* SERVERPARAMETER_HPP_ */
