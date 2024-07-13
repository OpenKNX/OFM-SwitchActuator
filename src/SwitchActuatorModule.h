#pragma once
#include "OpenKNX.h"
#include "SwitchActuatorChannel.h"
#include "hardware.h"
#include "knxprod.h"

#ifdef OPENKNX_SWA_IO_TCA_WIRE
  #include "TCA9555.h"
#endif

#define OPENKNX_SWA_FLASH_VERSION 0
#define OPENKNX_SWA_FLASH_MAGIC_WORD 3441922009

#define CH_SWITCH_DEBOUNCE 250

const uint8_t RELAY_SET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_SET_PINS};
const uint8_t RELAY_RESET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_RESET_PINS};

class SwitchActuatorModule : public OpenKNX::Module
{
  public:
    SwitchActuatorModule();
    ~SwitchActuatorModule();

    void processInputKo(GroupObject &iKo);
    void setup(bool configured);
    void loop();

    void doSwitchChannel(uint8_t channelIndex, bool active, bool syncSwitch = true);

    void writeFlash() override;
    void readFlash(const uint8_t* data, const uint16_t size) override;
    uint16_t flashSize() override;

    void savePower() override;
    bool restorePower() override;

    const std::string name() override;
    const std::string version() override;

  private:
    SwitchActuatorChannel *channel[SWA_ChannelCount];
    uint32_t chSwitchLastTrigger[8] = {};

#ifdef OPENKNX_SWA_IO_TCA_WIRE
    TCA9555 tca = TCA9555(OPENKNX_SWA_IO_TCA_ADDR, &OPENKNX_SWA_IO_TCA_WIRE);
#endif
};

extern SwitchActuatorModule openknxSwitchActuatorModule;