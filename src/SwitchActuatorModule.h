#pragma once
#include "OpenKNX.h"
#include "SwitchActuatorChannel.h"
#include "hardware.h"
#include "knxprod.h"

const uint8_t RELAY_SET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_SET_PINS};
const uint8_t RELAY_RESET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_RESET_PINS};

class SwitchActuatorModule : public OpenKNX::Module
{
  public:
    SwitchActuatorModule();
    ~SwitchActuatorModule();

    void processInputKo(GroupObject &iKo);
    void setup();
    void loop();

    const std::string name() override;
    const std::string version() override;

  private:
    SwitchActuatorChannel *channel[SWA_ChannelCount];
};

extern SwitchActuatorModule openknxSwitchActuatorModule;