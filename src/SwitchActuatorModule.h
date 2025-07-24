#pragma once
#include "OpenKNX.h"
#include "SwitchActuatorChannel.h"
#include "hardware.h"
#include "knxprod.h"

#define OPENKNX_SWA_FLASH_VERSION 0
#define OPENKNX_SWA_FLASH_MAGIC_WORD 3441922009

#define CH_SWITCH_DEBOUNCE 250

#ifdef OPENKNX_SWA_SET_PINS
  const uint16_t RELAY_SET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_SET_PINS};
  const uint16_t RELAY_RESET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_RESET_PINS};
#else
  const uint16_t RELAY_SET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {};
  const uint16_t RELAY_RESET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {};
#endif

#ifdef OPENKNX_SWA_STATUS_PINS
const uint16_t RELAY_STATUS_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_STATUS_PINS};
#endif
#ifdef OPENKNX_SWA_SWITCH_PINS
const uint16_t RELAY_SWITCH_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_SWITCH_PINS};
#endif

class SwitchActuatorModule : public OpenKNX::Module
{
  public:
    SwitchActuatorModule();
    ~SwitchActuatorModule();

    void processInputKo(GroupObject &iKo);
    void setup(bool configured);
    void loop();

    void doSwitchChannel(uint8_t channelIndex, bool active, bool syncSwitch = true);

    bool getChannelStatus(uint8_t channelIndex);

    void writeFlash() override;
    void readFlash(const uint8_t* data, const uint16_t size) override;
    uint16_t flashSize() override;

    void savePower() override;
    bool restorePower() override;

    const std::string name() override;
    const std::string version() override;

    bool processCommand(const std::string cmd, bool diagnoseKo) override;
    void showHelp() override;

  private:
    SwitchActuatorChannel *channel[OPENKNX_SWA_CHANNEL_COUNT];
    uint32_t chSwitchLastTrigger[OPENKNX_SWA_CHANNEL_COUNT] = {};
};

extern SwitchActuatorModule openknxSwitchActuatorModule;