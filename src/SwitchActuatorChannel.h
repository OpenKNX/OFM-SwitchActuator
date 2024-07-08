#pragma once
#include "OpenKNX.h"

class SwitchActuatorChannel : public OpenKNX::Channel
{
  private:
    const std::string name() override;
    bool statusDuringLock;
    uint32_t relayBistableImpulsTimer = 0;

    void doSwitch(bool active);
    void relaisOff();

  protected:

  public:
    SwitchActuatorChannel(uint8_t iChannelNumber);
    ~SwitchActuatorChannel();

    void processInputKo(GroupObject &iKo);
    void setup();
    void loop();
};