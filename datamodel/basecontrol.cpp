/*
 * basecontrol.cpp
 *
 *  Created on: 22.12.2019
 *      Author: rene
 */

#include "basecontrol.hpp"

namespace DM
{
  const std::string ControlResponse::sSuccess            = "Success";
  const std::string ControlResponse::sAlreadySelected    = "Already selected";
  const std::string ControlResponse::sAlreadyTargetState = "Already in target state";
  const std::string ControlResponse::sTimeout            = "Timeout";

  BaseControl::BaseControl(ControlModelFlags aControlmodel)
    : mRequireSelect(aControlmodel & REQUIRE_SELECT),
      mWaitForExecution(aControlmodel & VERIFY_EXECUTION),
      mIsSelected(false), mSelectionValue() {}

} /* namespace DM */
