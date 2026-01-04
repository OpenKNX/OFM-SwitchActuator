#pragma once
#include "OpenKNX.h"
#ifdef OPENKNX_SWA_BL0942_SPI
  #include "BL0942_SPI.h"

  #define OPENKNX_SWA_BL0942_INIT_DELAY 5000 // wait 5s to let capacitors charge first
  #define OPENKNX_SWA_BL0942_LOOP_DELAY 100
#endif

#define SWITCH_DEBOUNCE 250

class SwitchActuatorChannel : public OpenKNX::Channel
{
  private:
    const std::string name() override;
    bool statusDuringLock;
    uint32_t statusCyclicSendTimer = 0;
    uint32_t relayBistableImpulsTimer = 0;
    uint32_t turnOnDelayTimer = 0;
    uint32_t turnOffDelayTimer = 0;
    uint32_t switchLastTrigger = 0;

    void doSwitchInternal(bool active, bool syncSwitch = true);
    void processSwitchInput(bool newActive);
    void processLockInput(bool newActive);
    void processScene(uint8_t sceneNumber, bool learn);
    void relaisOff();

#ifdef OPENKNX_SWA_BL0942_SPI
    BL0942_SPI bl0942 = BL0942_SPI(14, &OPENKNX_SWA_BL0942_SPI);

    bool bl0942Initialized = false;
    uint32_t bl0942StartupDelay = 0;
    uint32_t bl0942UpdateTimer = 0;
    float lastReceivedPower = 0;
    float lastReceivedCurrent = 0;
    float lastReceivedVoltage = 0;
    float lastSentPower = 0;
    float lastSentCurrent = 0;
    float lastSentVoltage = 0;
    uint32_t powerCyclicSendTimer = 0;
    uint32_t currentCyclicSendTimer = 0;
    uint32_t voltageCyclicSendTimer = 0;

    uint32_t _debugTimer = 0;

    static SwitchActuatorChannel* switchActuatorChannelInstance;
    static void bl0942ChannelSelector(bool active);
    void setChannelSelectorBl0942(bool active);
    void initBl0942();
#endif

  protected:

  public:
    SwitchActuatorChannel(uint8_t iChannelNumber);
    ~SwitchActuatorChannel();

    void processInputKo(GroupObject &iKo) override;
    void setup(bool configured) override;
    void loop() override;

    void doSwitch(bool active, bool syncSwitch = true);
    bool isRelayActive();
    void savePower();
    bool restorePower();

#ifdef OPENKNX_SWA_BL0942_SPI
    float getPower();
    float getCurrent();
#endif
};