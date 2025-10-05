#pragma once
#include "OpenKNX.h"
#ifdef OPENKNX_SWA_BL0942_SPI
  #include "BL0942.h"
#endif

class SwitchActuatorChannel : public OpenKNX::Channel
{
  private:
    const std::string name() override;
    bool statusDuringLock;
    uint32_t statusCyclicSendTimer = 0;
    uint32_t relayBistableImpulsTimer = 0;
    uint32_t turnOnDelayTimer = 0;
    uint32_t turnOffDelayTimer = 0;

#ifdef OPENKNX_SWA_BL0942_SPI
    bl0942::BL0942 bl0942 = bl0942::BL0942(OPENKNX_SWA_BL0942_SPI);
#endif

    void doSwitchInternal(bool active, bool syncSwitch = true);
    void processSwitchInput(bool newActive);
    void processLockInput(bool newActive);
    void processScene(uint8_t sceneNumber, bool learn);
    void relaisOff();

#ifdef OPENKNX_SWA_BL0942_SPI
    void dataReceivedBl0942(bl0942::SensorData &data);
    void setChannelSelectorBl0942(bool active);
    void initBl0942();
#endif

    uint32_t _debugTimer = 0;

  protected:

  public:
    SwitchActuatorChannel(uint8_t iChannelNumber);
    ~SwitchActuatorChannel();

    void processInputKo(GroupObject &iKo) override;
    void setup(bool configured) override;
    void loop(bool configured) override;

    void doSwitch(bool active, bool syncSwitch = true);
    bool isRelayActive();
    void savePower();
    bool restorePower();
};