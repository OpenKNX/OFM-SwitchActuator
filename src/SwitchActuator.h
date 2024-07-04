#pragma once
#include "OpenKNX.h"
#include "SwitchActuatorChannel.h"
#include "hardware.h"
#include "knxprod.h"

class SwitchActuator : public OpenKNX::Module
{
  public:
    SwitchActuator();
    ~SwitchActuator();

    void processInputKo(GroupObject &iKo);
    void setup();
    void loop();

    const std::string name() override;
    const std::string version() override;

  private:
    SwitchActuatorChannel *channel[SWA_ChannelCount];
};

extern SwitchActuator openknxSwitchActuatorModule;