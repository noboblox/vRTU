/*
 * httpconnection.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: blox
 */

#include "httpconnection.hpp"

#include <iostream>

namespace HTTP = boost::beast::http;

namespace VRTU
{


  HttpConnection::HttpConnection(TcpSocket&& arrSocket, DestroyHandler aSignalDestroy)
    : mSocket(std::move(arrSocket)), mReadBuffer(MAX_REQUEST_BYTES), mRequest(),
      SignalRequestRead(),
      SignalDestroy(aSignalDestroy)
  {
  }

  HttpConnection::~HttpConnection()
  {
  }

  void
  HttpConnection::StartRead(RequestHandler aSignalRequest)
  {
    SignalRequestRead = aSignalRequest;
    StartReadInternal();
  }

  void
  HttpConnection::Stop()
  {
    mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    mSocket.close();
    SignalDestroy(*this);
  }

  void
  HttpConnection::StartReadInternal()
  {
    mRequest.clear();
    HTTP::async_read(mSocket, mReadBuffer, mRequest, std::bind(&HttpConnection::OnReadFinishedInternal,
                                                                   this, std::placeholders::_1, std::placeholders::_2));
  }

  void
  HttpConnection::OnReadFinishedInternal(boost::system::error_code aError, size_t aBytesRead)
  {
    if (aError)
    {
      std::cout << "Error reading HTTP request: " << aError << std::endl;
      Stop();
      return;
    }

    std::cout << "Received request of size: " << aBytesRead << std::endl
              << "Method: " << mRequest.method() << std::endl
              << "Target: " << mRequest.target() << std::endl;


    // TODO Parse into a TC::REST::BaseRequest
    // For now: Generic response (blocking)
    HTTP::response<HTTP::string_body> response;
    response.version(11);
    response.result(HTTP::status::ok);
    response.set(HTTP::field::server, "rtuapp");
    response.prepare_payload();

    boost::system::error_code error;
    HTTP::write(mSocket, response, error);

    if (error)
    {
      std::cout << "Error writing response: " << error << std::endl;
      Stop();
      return;
    }

    StartReadInternal(); // Next message
  }

} /* namespace VRTU */
