#include "iec104quality.hpp"

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