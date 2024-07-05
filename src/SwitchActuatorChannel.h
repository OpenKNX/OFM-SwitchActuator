#pragma once
#include "OpenKNX.h"

class SwitchActuatorChannel : public OpenKNX::Channel
{
  private:
    const std::string name() override;
    bool statusDuringLock;

  protected:

  public:
    SwitchActuatorChannel(uint8_t iChannelNumber);
    ~SwitchActuatorChannel();

    void processInputKo(GroupObject &iKo);
    void doSwitch(bool active);
    void setup();
    void loop();
};