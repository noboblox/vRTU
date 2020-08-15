/*
 * rtuapp.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: blox
 */

#include "rtuapp.hpp"

#include <memory>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "startarg.hpp"

int
main(int argc, char* argv[])
{
  try
  {
    VRTU::RTUApp app(argc, argv);
    app.Run();

    return 0;
  }
  catch (std::exception& e) {std::cout << "Unhandled exception:" << e.what() << " - Shutdown app." << std::endl;}
  catch (...) {std::cout << "Unhandled exception: <unknown> - Shutdown app." << std::endl;}

  return -1;
}

namespace VRTU
{
  RTUApp::RTUApp(int argc, char* argv[])
    : mArgParser(), mContext(), mAcceptor(mContext),
      mFlagHelp(false), mPort(26000), mIP("::1")
  {
    mArgParser.Register(std::make_unique<FlagArg>
                         ("-h", "--help", mFlagHelp, StartArg::OPTIONAL,
                          "Display the help message."));
    mArgParser.Register(std::make_unique<ValueArg<uint16_t>>
                         ("-p", "--port", mPort, StartArg::OPTIONAL,
                          "Set the listening port for API requests. (Default = 26000)"));
    mArgParser.Register(std::make_unique<IpArg>
                         ("-I", "--ip", mIP, StartArg::OPTIONAL,
                          "Set the listening IP for API requests. (Default = ::1)"));

    if (!mArgParser.Parse(argc, argv))
      mFlagHelp = true;
  }

  RTUApp::~RTUApp()
  {
  }

  void
  RTUApp::RunListener()
  {
    std::cout << "Try to startup server listener on " << mIP << ":" << mPort << " ..." << std::endl;

    boost::asio::ip::tcp::resolver resolver(mContext);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(mIP, std::to_string(mPort)).begin();

    mAcceptor.open(endpoint.protocol());
    mAcceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    mAcceptor.bind(endpoint);
    mAcceptor.listen();

    std::cout << "... Done." << std::endl;

    StartAccept();
    mContext.run(); // Start handling async calls
  }

  void
  RTUApp::StartAccept()
  {
    std::cout << "Waiting for a new connect request ..." << std::endl;

    mAcceptor.async_accept(
        [this](boost::system::error_code aError, boost::asio::ip::tcp::socket&& arrSocket)
        {
          if (!mAcceptor.is_open())
            return;

          try
          {
            boost::asio::ip::tcp::endpoint client = arrSocket.remote_endpoint();

            if (!aError)
            {
              std::cout << "... Accept new connection from remote host " << client.address() << ":" << client.port() << "." << std::endl;
              std::cout << "TODO" << std::endl;
              // TODO: Accept a new connection
            }
            else
            {
              std::cout << "... Reject connection from remote host " << client.address() << ":" << client.port()
                        << " (Errorcode: " << aError << ")." << std::endl;
            }
          }
          catch (boost::system::system_error& e) {e.what();}



          StartAccept(); // Wait for the next connection
        });
  }

  void
  RTUApp::Run()
  {
    if (mFlagHelp)
    {
      PrintHelp(std::cout);
      return;
    }

    RunListener();
  }

  void
  RTUApp::PrintHelp(std::ostream& arOut) const
  {
    arOut << "RTUApp (stub) - API server for virtual RTU simulation" << std::endl
          << "VALID ARGUMENTS: " << std::endl
          << std::setw(10) << std::left << "SHORTCUT"
          << std::setw(30) << std::left << "FULL FORMAT"
          << std::setw(10) << std::left << "REQUIRED"
          << std::setw(50) << std::left << "DESCRIPTION" << std::endl;

    const auto& rp_args = mArgParser.GetRegisteredArgs();

    for (const auto& p_arg : rp_args)
      arOut << *p_arg << std::endl;
  }

} /* namespace VRTU */
