/*
 * telecontrolserverinterface.hpp
 *
 *  Created on: 16.12.2019
 *      Author: rene
 */

#ifndef TC_TELECONTROLSERVERINTERFACE_HPP_
#define TC_TELECONTROLSERVERINTERFACE_HPP_

#include <functional>

#include "basepropertylist.hpp"

namespace TC
{

  struct BaseServerParameter;
  class BaseServerStatus;

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
    virtual void RegisterControlData(const std::string& arId, const BasePropertyList& arDefinition) = 0;
    virtual void RegisterStatusData(const std::string& arId, const BasePropertyList& arDefinition) = 0;
    virtual void UnregisterDatapoint(const std::string& arId) = 0;

    virtual void StartServer() = 0;
    virtual void StopServer() = 0;

  public:
    //using SelectRequest  = std::function<bool(const DM::BaseDataValue&)>;
    //using OperateRequest = std::function<bool(const DM::BaseDataValue&)>;
    //using CancelRequest  = std::function<bool()>;
    //using ReadRequest    = std::function<DM::BaseDataValue()>;

};

} /* namespace TC */

#endif /* TC_TELECONTROLSERVERINTERFACE_HPP_ */
