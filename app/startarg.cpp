/*
 * startarg.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: blox
 */

#include "startarg.hpp"

#include <algorithm>
#include <boost/asio/ip/address.hpp>

namespace VRTU
{
  StartArg::StartArg(const std::string& arShortKey, const std::string& arLongKey, const std::string& arDescription,
                     RequirementLevel aLevel)
     : mShortKey(arShortKey), mLongKey(arLongKey), mDesc(arDescription), mLevel(aLevel), mSet(false)
  {
  }

  StartArg::~StartArg()
  {
  }

  std::ostream&
  operator<<(std::ostream& arStream, const StartArg& arArg)
  {
    arArg.Print(arStream);
    return arStream;
  }

  FlagArg::FlagArg(const std::string& arShortKey, const std::string& arLongKey,
                   bool& arDestination, RequirementLevel aLevel, const std::string& arDescription)
    : StartArg(arShortKey, arLongKey, arDescription, aLevel), mrDestination(arDestination)
  {
  }

  bool
  FlagArg::Read(const std::vector<std::string>& arArgs, int& arCurrentIndex)
  {
    if (IsMatch(arArgs[arCurrentIndex]))
    {
      mrDestination = true;
      SetStatus(true);
      ++arCurrentIndex;

      return true;
    }

    return false;
  }

  void
  FlagArg::Print(std::ostream& arStream) const
  {
    arStream << std::setw(10) << std::left << GetShortKey()
             << std::setw(30) << std::left << GetLongKey()
             << std::setw(10) << std::left << GetRequiredString()
             << std::setw(50) << std::left << GetDescription();
  }

  IpArg::IpArg(const std::string& arShortKey, const std::string& arLongKey,
        std::string& arDestination, RequirementLevel aLevel, const std::string& arDescription)
    : StartArg(arShortKey, arLongKey, arDescription, aLevel), mrDestination(arDestination)
  {
  }

  bool
  IpArg::Read(const std::vector<std::string>& arArgs, int& arCurrentIndex)
  {
    if (!IsMatch(arArgs[arCurrentIndex]))
      return false;

    int index = arCurrentIndex; // Only modify index if successfully consumed
    ++index;

    if (index >= arArgs.size())
      return false;

    boost::system::error_code error;
    auto ip = boost::asio::ip::make_address(arArgs[index], error);

    if (error)
      return false;

    arCurrentIndex = ++index;
    mrDestination = ip.to_string(error);
    SetStatus(true);

    if (error)
      throw std::runtime_error("Failed to validate provided IP address.");

    return true;
  }

  void
  IpArg::Print(std::ostream& arStream) const
  {
    std::string value_desc = GetLongKey();
    value_desc.append(" <ipv4> OR <ipv6>");

    arStream << std::setw(10) << std::left << GetShortKey()
             << std::setw(30) << std::left << value_desc
             << std::setw(10) << std::left << GetRequiredString()
             << std::setw(50) << std::left << GetDescription();
  }

  void
  StartArgParser::Register(SpStartArg&& arrSpArg)
  {
    mArgs.push_back(std::move(arrSpArg));
  }

  bool
  StartArgParser::FoundAllRequired() const
  {
    auto it_missing = std::find_if(mArgs.cbegin(), mArgs.cend(),
        [](const SpStartArg& arSpArg) -> bool
        {
          return (arSpArg->IsRequired() && !arSpArg->IsSet());
        });

    if (it_missing != mArgs.cend())
      return false;

    return true;
  }

  bool
  StartArgParser::Parse(int argc, char* argv[])
  {
    std::vector<std::string> arg_list;
    arg_list.reserve(argc);

    for (int i = 1; i < argc; ++i) // argv[0] is not considered
      arg_list.emplace_back(argv[i]);

    int i = 0;
    while (i < arg_list.size())
    {
      bool consumed = false;
      for (auto& p_arg : mArgs)
      {
        if (p_arg->Read(arg_list, i)) // Increment on consumption
        {
          consumed = true;
          break;
        }
      }

      if (!consumed)
        return false;
    }

    if (!FoundAllRequired())
      return false;

    return true;
  }

} /* namespace VRTU */
