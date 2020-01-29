#ifndef IEC104_QUALITY_HPP_
#define IEC104_QUALITY_HPP_

#include <cstdint>
#include <map>
#include <string>
#include <stdexcept>

namespace IEC104
{
  class Quality
  {
  public:
    explicit Quality()
      : mFlags(sFlagInvalid) {}

    explicit Quality(uint8_t aFlags)
      : mFlags(aFlags) {}

    explicit Quality(const std::string& arName)
      : mFlags(sValueGood)
    {
      if (arName == sStringGood)
        return;

      FlagMap::const_iterator resultIt = msFlagNames.find(arName);
      
      if (resultIt == msFlagNames.cend())
        throw std::invalid_argument("Could not derive the quality flag from the provided string.");
      mFlags = resultIt->second; 
    }

    inline bool IsGood()        const { return mFlags == sValueGood; }
    inline bool IsOverflow()    const { return mFlags  & sFlagOverflow; }
    inline bool IsReserved()    const { return mFlags  & sFlagReserved; }
    inline bool IsTimeout()     const { return mFlags  & sFlagTimeout; }
    inline bool IsBlocked()     const { return mFlags  & sFlagBlocked; }
    inline bool IsSubstituted() const { return mFlags  & sFlagSubstituted; }
    inline bool IsNonTopical()  const { return mFlags  & sFlagNonTopical; }
    inline bool IsInvalid()     const { return mFlags  & sFlagInvalid; }

    inline void SetToGood()                      { mFlags = sValueGood; }
    inline void EditFlagOverflow(bool aValue)    { aValue ? mFlags |= sFlagOverflow    : mFlags &= (~sFlagOverflow);    }
    inline void EditFlagReserved(bool aValue)    { aValue ? mFlags |= sFlagReserved    : mFlags &= (~sFlagReserved);    }
    inline void EditFlagTimeout(bool aValue)     { aValue ? mFlags |= sFlagTimeout     : mFlags &= (~sFlagTimeout);     }
    inline void EditFlagBlocked(bool aValue)     { aValue ? mFlags |= sFlagBlocked     : mFlags &= (~sFlagBlocked);     }
    inline void EditFlagSubstituted(bool aValue) { aValue ? mFlags |= sFlagSubstituted : mFlags &= (~sFlagSubstituted); }
    inline void EditFlagNonTopical(bool aValue)  { aValue ? mFlags |= sFlagNonTopical  : mFlags &= (~sFlagNonTopical);  }
    inline void EditFlagInvalid(bool aValue)     { aValue ? mFlags |= sFlagInvalid     : mFlags &= (~sFlagInvalid);     }

    inline uint8_t GetInt() const { return mFlags; }

    const bool operator==(const Quality& arOther) const { return mFlags == arOther.mFlags; }
    const bool operator!=(const Quality& arOther) const { return !operator==(arOther); }


  public:
    static constexpr uint8_t sValueGood        = 0x0;
    static constexpr const char* sStringGood   = "good";

    static constexpr uint8_t sFlagOverflow    = 0x01;
    static constexpr uint8_t sFlagReserved    = 0x04;
    static constexpr uint8_t sFlagTimeout     = 0x08;
    static constexpr uint8_t sFlagBlocked     = 0x10;
    static constexpr uint8_t sFlagSubstituted = 0x20;
    static constexpr uint8_t sFlagNonTopical  = 0x40;
    static constexpr uint8_t sFlagInvalid     = 0x80;

    static constexpr const char* sStringOverflow    = "overflow";
    static constexpr const char* sStringReserved    = "reserved";
    static constexpr const char* sStringTimeout     = "timeout";
    static constexpr const char* sStringBlocked     = "blocked";
    static constexpr const char* sStringSubstituted = "substituted";
    static constexpr const char* sStringNonTopical  = "non-topical";
    static constexpr const char* sStringInvalid     = "invalid";
    
    using FlagMap = const std::map<const std::string, const uint8_t>;
  private:
    static FlagMap msFlagNames;

  private:
    uint8_t mFlags;
  };
}

#endif // !IEC104_QUALITY_HPP_