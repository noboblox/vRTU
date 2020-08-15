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
  catch (std::exception& e) {std::cout << "Unhandled expception:" << e.what() << " - Shutdown app." << std::endl;}
  catch (...) {std::cout << "Unhandled exception: <unknown> - Shutdown app." << std::endl;}

  return -1;
}

namespace VRTU
{
  RTUApp::RTUApp(int argc, char* argv[])
    : mArgParser(),
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
  RTUApp::Run()
  {
    if (mFlagHelp)
    {
      PrintHelp(std::cout);
      return;
    }

    std::cout << "TODO..." << std::endl;
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
