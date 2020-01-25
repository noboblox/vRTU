#ifndef TC_IEC104SERVER_HPP_
#define TC_IEC104SERVER_HPP_

#include "telecontrolserverinterface.hpp"
#include "serverparameter.hpp"

namespace TC
{
  class Iec104ServerImpl;
  class Iec104RawMessage;

  class Iec104Server: public TelecontrolServerInterface
  {
  public:
    Iec104Server(const Iec104ServerParameter& arSettings);
    virtual ~Iec104Server();

  public:
    Iec104Server(const Iec104Server&) = delete;
    Iec104Server(Iec104Server&&) = delete;
    Iec104Server& operator=(const Iec104Server&) = delete;
    Iec104Server& operator=(Iec104Server&&) = delete;

  public:
    virtual void RegisterControlData(const std::string& arId, const BasePropertyList& arDefinition) override;
    virtual void RegisterStatusData(const std::string& arId, const BasePropertyList& arDefinition) override;
    virtual void UnregisterDatapoint(const std::string& arId) override;

    virtual void StartServer() override;
    virtual void StopServer() override;

  private:
    Iec104ServerImpl* mpImpl;
  };

} /* namespace TC */

#endif /* TC_IEC104SERVER_HPP_ */
