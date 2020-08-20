/*
 * httpconnection.hpp
 *
 *  Created on: Aug 15, 2020
 *      Author: blox
 */

#ifndef HTTPCONNECTION_HPP_
#define HTTPCONNECTION_HPP_

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http.hpp>

#include "baserequest.hpp"

namespace VRTU
{

  class HttpConnection
  {
  public:
    /**
     * @brief Request handler called when a full HTTP request was read
     *
     * @retval true Successful execution
     * @retval false Error
     */
    using RequestHandler = std::function<bool(TC::REST::BaseRequest&&)>;
    using DestroyHandler = std::function<void(HttpConnection&)>;

    using TcpSocket  = boost::asio::ip::tcp::socket;
    using RawRequest = boost::beast::http::request<boost::beast::http::string_body>;

    explicit HttpConnection(TcpSocket&& arrSocket, DestroyHandler aSignalDestroy);
    ~HttpConnection();

    /// Initiate async request reading
    void StartRead(RequestHandler aSignalRequest);
    /// Stop the connection. The socket is closed. The connection should not be used anymore
    void Stop();

    HttpConnection(const HttpConnection& arOther) = delete;
    HttpConnection(HttpConnection&& arrOther) = delete;
    HttpConnection& operator=(const HttpConnection& arOther) = delete;
    HttpConnection& operator=(HttpConnection&& arrOther) = delete;

    static constexpr size_t MAX_REQUEST_BYTES = 1 * 1024 * 1024;
    static constexpr size_t MAX_RESPONSE_BYTES = 1 * 1024 * 1024;

  private:
    void StartReadInternal();
    void OnReadFinishedInternal(boost::system::error_code aError, size_t aBytesRead);

    TcpSocket mSocket;
    boost::beast::flat_buffer mReadBuffer;
    RawRequest mRequest;

    RequestHandler SignalRequestRead;
    DestroyHandler SignalDestroy;
  };

} /* namespace VRTU */

#endif /* HTTPCONNECTION_HPP_ */
