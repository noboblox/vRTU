/*
 * basecontrol.hpp
 *
 *  Created on: 22.12.2019
 *      Author: rene
 */

#ifndef BASECONTROL_HPP_
#define BASECONTROL_HPP_

#include <chrono>
#include <mutex>
#include <atomic>
#include <thread>
#include <string>

namespace DM
{

  class BaseValue
  {
// stub
  };

  class ControlResponse
  {
  public:
    ControlResponse() : mMessage(sSuccess) {}
    explicit ControlResponse(const std::string& arMessage) : mMessage(arMessage) {}
    ~ControlResponse();

    bool IsSuccess() const {return mMessage == sSuccess;}
    const std::string GetMessage() {return mMessage;}

    static const std::string sSuccess;
    static const std::string sAlreadySelected;
    static const std::string sAlreadyTargetState;
    static const std::string sTimeout;

  private:
    std::string mMessage;
  };

  class BaseControl
  {
  public:
    enum ControlModelFlags
    {
      REQUIRE_SELECT = 0x01, //!< Any control request shall call Select() first.
      VERIFY_EXECUTION = 0x02 //!< The command will wait for status change until report the command success
    };

  public:
    explicit BaseControl(ControlModelFlags aControlmodel);
    virtual ~BaseControl() = 0;

    BaseControl(const BaseControl&) = delete;
    BaseControl(BaseControl&&) = delete;
    BaseControl& operator=(const BaseControl&) = delete;
    BaseControl& operator=(BaseControl&&) = delete;

    // TODO async / user interactive
    virtual bool RequestSelect(const DM::BaseValue& arTargetValue) = 0;
    virtual bool RequestOperate(const DM::BaseValue& arTargetValue) = 0;
    virtual void RequestCancel() = 0;

  protected:
    virtual void DoSelect(const DM::BaseValue& arSelectValue) = 0;
    virtual void DoOperate(const DM::BaseValue& arSelectValue) = 0; //!< Implemented by derived class which operates on the real value

  private:
    bool mRequireSelect;
    bool mWaitForExecution;

    bool mIsSelected;
    DM::BaseValue mSelectionValue;
    std::chrono::milliseconds mSimulatedOperateDelay;
    std::chrono::milliseconds mSelectionTimeout;

    mutable std::mutex mMainLock;
  };

} /* namespace DM */

#endif /* BASECONTROL_HPP_ */
