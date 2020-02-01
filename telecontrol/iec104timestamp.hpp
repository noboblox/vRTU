#ifndef _IEC104_TIMESTAMP_HPP_
#define _IEC104_TIMESTAMP_HPP_

#include <string>
#include <locale>

#include <boost/date_time/posix_time/posix_time.hpp>

extern "C"
{
#include "iec60870_common.h"
}

namespace IEC104
{
  class Timestamp
  {
  public:
    static constexpr const char* sNowToken = "now";

    Timestamp(); //!< Default construct using the current time as UTC.
    
     //! Formatted UTC timestamp with optionional fraction.
     //!< e.g. 2020-09-15 13:24:15.001
    explicit Timestamp(const std::string& arTimestamp);
    std::string GetString() const;
    sCP56Time2a Encode56() const;

    size_t GetMilliseconds() const;

    inline const bool operator==(const Timestamp& arOther) const { return mTime == arOther.mTime; }
    inline const bool operator!=(const Timestamp& arOther) const { return !operator==(arOther); }
    inline const bool operator< (const Timestamp& arOther) const { return mTime < arOther.mTime; }
    inline const bool operator>=(const Timestamp& arOther) const { return !operator<(arOther); }
    inline const bool operator> (const Timestamp& arOther) const { return !operator<(arOther) && !operator==(arOther); }
    inline const bool operator<=(const Timestamp& arOther) const { return  operator<(arOther) ||  operator==(arOther); }

  private:
    boost::posix_time::ptime mTime;
  };
}
#endif

