#pragma once
#include "OpenKNX.h"

class SwitchActuator;

class SwitchActuatorChannel : public OpenKNX::Channel
{
  private:
    const std::string name() override;

  protected:

  public:
    SwitchActuatorChannel(uint8_t iChannelNumber);
    ~SwitchActuatorChannel();

    void processInputKo(GroupObject &iKo, int8_t iKoIndex = -1);
    void setup();
    void loop();
};