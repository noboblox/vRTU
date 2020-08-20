/*
 * RTUApp.h
 *
 *  Created on: Aug 11, 2020
 *      Author: blox
 */

#ifndef RTUAPP_H_
#define RTUAPP_H_

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <set>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "startarg.hpp"

namespace VRTU
{
  class HttpConnection;

  class RTUApp
  {
  public:
    using ConnectionPtr = std::unique_ptr<HttpConnection>;

    RTUApp(int argc, char* argv[]);
    ~RTUApp() noexcept;

    void Run();
    void PrintHelp(std::ostream& arOut) const;

    RTUApp(const RTUApp& arOther) = delete;
    RTUApp(RTUApp&& arOther) = delete;
    RTUApp& operator=(const RTUApp& arOther) = delete;
    RTUApp& operator=(RTUApp&& arOther) = delete;

  private:
    void RunListener();
    void StartAccept();
    void PrepareConnectionDestroy(HttpConnection& arConnection);

  private:
    StartArgParser mArgParser;
    boost::asio::io_context mContext;
    boost::asio::ip::tcp::acceptor mAcceptor;
    std::set<ConnectionPtr> mConnections;
    bool mFlagHelp;
    uint16_t mPort;
    std::string mIP;

  };

} /* namespace VRTU */

#endif /* RTUAPP_H_ */
