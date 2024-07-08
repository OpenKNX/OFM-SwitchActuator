#pragma once
#include "OpenKNX.h"
#include "SwitchActuatorChannel.h"
#include "hardware.h"
#include "knxprod.h"

#define OPENKNX_SWA_FLASH_VERSION 0
#define OPENKNX_SWA_FLASH_MAGIC_WORD 3441922009

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

    void writeFlash() override;
    void readFlash(const uint8_t* data, const uint16_t size) override;
    uint16_t flashSize() override;

    void savePower() override;
    bool restorePower() override;

    const std::string name() override;
    const std::string version() override;

  private:
    SwitchActuatorChannel *channel[SWA_ChannelCount];
};

extern SwitchActuatorModule openknxSwitchActuatorModule;