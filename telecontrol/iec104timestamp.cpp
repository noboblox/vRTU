#include "iec104timestamp.hpp"

namespace IEC104
{
  constexpr const char* Timestamp::sNowToken;
 
  Timestamp::Timestamp()
    : mTime(boost::posix_time::microsec_clock::universal_time()) {}

  Timestamp::Timestamp(const std::string& arString)
    : mTime(boost::posix_time::from_iso_extended_string(arString)) {}

  std::string
  Timestamp::GetString() const
  {
    return boost::posix_time::to_iso_extended_string(mTime);
  }

  size_t Timestamp::GetMilliseconds() const
  {
    static const boost::posix_time::ptime cEpochZero(boost::gregorian::date(1970,01,01));

    const boost::posix_time::time_duration unixTime(mTime - cEpochZero);
    return unixTime.total_milliseconds();
  }

  sCP56Time2a
  Timestamp::Encode56() const
  {
    sCP56Time2a result{};
    CP56Time2a_setFromMsTimestamp(&result, GetMilliseconds());
    return result;
  }

}