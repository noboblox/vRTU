/*
 * telecontrolserverinterface.hpp
 *
 *  Created on: 16.12.2019
 *      Author: rene
 */

#ifndef TC_TELECONTROLSERVERINTERFACE_HPP_
#define TC_TELECONTROLSERVERINTERFACE_HPP_

#include <functional>

namespace DM
{
  class BaseGridData;
  class BaseDataValue;
}

namespace TC
{

  class BaseServerParameter;
  class BaseServerStatus;

  class BaseTelegramId;
  class BaseControlId;
  class BaseStatusId;


  class TelecontrolServerInterface
  {
  public:
    explicit TelecontrolServerInterface() {}
    virtual ~TelecontrolServerInterface() {}

  public:
    TelecontrolServerInterface(const TelecontrolServerInterface&) = delete;
    TelecontrolServerInterface& operator=(const TelecontrolServerInterface&) = delete;
    TelecontrolServerInterface(TelecontrolServerInterface&&) = delete;
    TelecontrolServerInterface& operator=(TelecontrolServerInterface&&) = delete;

  public:
    virtual void RegisterControllable(const BaseControlId& arControlId, DM::BaseGridData& arData) = 0;
    virtual void RegisterStatusOnly(const BaseStatusId& arStatusId, DM::BaseGridData& arData) = 0;
    virtual void UnregisterTelegram(const BaseTelegramId& arId) = 0;

    virtual void StartServer() = 0;
    virtual void StopServer() = 0;

  public:
    virtual void OnStatusChange(const DM::BaseGridData& arData) const = 0;
    using SelectRequest  = std::function<bool(const DM::BaseDataValue&)>;
    using OperateRequest = std::function<bool(const DM::BaseDataValue&)>;
    using CancelRequest  = std::function<bool()>;
    using ReadRequest    = std::function<DM::BaseDataValue()>;

};

} /* namespace TC */

#endif /* TC_TELECONTROLSERVERINTERFACE_HPP_ */
