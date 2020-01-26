#ifndef TC_IEC104SERVER_HPP_
#define TC_IEC104SERVER_HPP_

#include "telecontrolserverinterface.hpp"
#include "serverparameter.hpp"

namespace IEC104
{
  class Iec104ServerImpl;
  class Iec104RawMessage;

  class Iec104Server: public TC::TelecontrolServerInterface
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
    virtual void RegisterControlData(const TC::BasePropertyList& arDefinition) override;
    virtual void RegisterStatusData(const TC::BasePropertyList& arDefinition) override;
    virtual void UnregisterData(const std::string& arId) override;

    virtual size_t CountStatusData() const override;
    virtual size_t CountControlData() const override;

    virtual void StartServer() override;
    virtual void StopServer() override;

  private:
    Iec104ServerImpl* mpImpl;
  };

} /* namespace TC */

#endif /* TC_IEC104SERVER_HPP_ */
