#pragma once
#include "OpenKNX.h"
#include "GPIOModule.h"
#include "SwitchActuatorChannel.h"
#include "hardware.h"
#include "knxprod.h"

#define OPENKNX_SWA_FLASH_VERSION 0
#define OPENKNX_SWA_FLASH_MAGIC_WORD 3441922009

#define CH_SWITCH_DEBOUNCE 250

#define GPIO_OUTPUT_ON  OPENKNX_SWA_GPIO_OUTPUT_ACTIVE_ON == HIGH ? HIGH : LOW
#define GPIO_OUTPUT_OFF OPENKNX_SWA_GPIO_OUTPUT_ACTIVE_ON == HIGH ? LOW : HIGH
#define GPIO_INPUT_ON   OPENKNX_SWA_GPIO_INPUT_ACTIVE_ON == HIGH ? HIGH : LOW
#define GPIO_INPUT_OFF  OPENKNX_SWA_GPIO_INPUT_ACTIVE_ON == HIGH ? LOW : HIGH

#ifdef OPENKNX_SWA_SET_PINS
  const uint8_t RELAY_SET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_SET_PINS};
  const uint8_t RELAY_RESET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {OPENKNX_SWA_RESET_PINS};
#else
  const uint8_t RELAY_SET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {};
  const uint8_t RELAY_RESET_PINS[OPENKNX_SWA_CHANNEL_COUNT] = {};
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

    void writeFlash() override;
    void readFlash(const uint8_t* data, const uint16_t size) override;
    uint16_t flashSize() override;

    void savePower() override;
    bool restorePower() override;

    const std::string name() override;
    const std::string version() override;

  private:
    SwitchActuatorChannel *channel[OPENKNX_SWA_CHANNEL_COUNT];
    uint32_t chSwitchLastTrigger[OPENKNX_SWA_CHANNEL_COUNT] = {};
};

extern SwitchActuatorModule openknxSwitchActuatorModule;