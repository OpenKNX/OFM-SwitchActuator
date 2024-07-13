#pragma once
#include "OpenKNX.h"

#define RELAY_GPIO_SET_ON    OPENKNX_SWA_SET_ACTIVE_ON == HIGH ? HIGH : LOW
#define RELAY_GPIO_SET_OFF   OPENKNX_SWA_SET_ACTIVE_ON == HIGH ? LOW : HIGH
#define RELAY_GPIO_RESET_ON  OPENKNX_SWA_RESET_ACTIVE_ON == HIGH ? HIGH : LOW
#define RELAY_GPIO_RESET_OFF OPENKNX_SWA_RESET_ACTIVE_ON == HIGH ? LOW : HIGH

class SwitchActuatorChannel : public OpenKNX::Channel
{
  private:
    const std::string name() override;
    bool statusDuringLock;
    uint32_t statusCyclicSendTimer = 0;
    uint32_t relayBistableImpulsTimer = 0;

    void processSwitchInput(bool newActive);
    void relaisOff();

  protected:

  public:
    SwitchActuatorChannel(uint8_t iChannelNumber);
    ~SwitchActuatorChannel();

    void processInputKo(GroupObject &iKo);
    void setup(bool configured);
    void loop();

    void doSwitch(bool active, bool syncSwitch = true);
    bool isRelayActive();
    void savePower();
    bool restorePower();
};