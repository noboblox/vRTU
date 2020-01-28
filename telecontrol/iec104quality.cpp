#include "iec104quality.hpp"

constexpr uint8_t IEC104::Quality::sValueGood;
constexpr uint8_t IEC104::Quality::sFlagOverflow;
constexpr uint8_t IEC104::Quality::sFlagReserved;
constexpr uint8_t IEC104::Quality::sFlagTimeout;
constexpr uint8_t IEC104::Quality::sFlagBlocked;
constexpr uint8_t IEC104::Quality::sFlagSubstituted;
constexpr uint8_t IEC104::Quality::sFlagNonTopical;
constexpr uint8_t IEC104::Quality::sFlagInvalid;

constexpr const char* IEC104::Quality::sStringGood;
constexpr const char* IEC104::Quality::sStringOverflow;
constexpr const char* IEC104::Quality::sStringReserved;
constexpr const char* IEC104::Quality::sStringTimeout;
constexpr const char* IEC104::Quality::sStringBlocked;
constexpr const char* IEC104::Quality::sStringSubstituted;
constexpr const char* IEC104::Quality::sStringNonTopical;
constexpr const char* IEC104::Quality::sStringInvalid;

IEC104::Quality::FlagMap IEC104::Quality::msFlagNames
{
  { sStringOverflow,    sFlagOverflow },
  { sStringReserved,    sFlagReserved },
  { sStringTimeout,     sFlagTimeout },
  { sStringBlocked,     sFlagBlocked },
  { sStringSubstituted, sFlagSubstituted },
  { sStringNonTopical,  sFlagNonTopical },
  { sStringInvalid,     sFlagInvalid }
};