#include "SwitchActuatorChannel.h"
#include "SwitchActuatorModule.h"

SwitchActuatorChannel::SwitchActuatorChannel(uint8_t iChannelNumber/*, SwitchActuatorModule* switchActuatorModulePtr*/)
{
    _channelIndex = iChannelNumber;
}

SwitchActuatorChannel::~SwitchActuatorChannel() {}

void SwitchActuatorChannel::processInputKo(GroupObject &ko)
{
    if (ParamSWA_ChActive != 1)
    {
        logTraceP("processInputKo: channel not active (%u)", ParamSWA_ChActive);
        return;
    }

    logDebugP("processInputKo: channel %u", _channelIndex);
    logIndentUp();

    bool newActive;
    switch (ko.asap())
    {
        case SWA_KoCentralFunction:
            if (ParamSWA_ChCentralFunction)
            {
                newActive = ko.value(DPT_Switch);
                logDebugP("SWA_KoCentralFunction: %u", newActive);

                processSwitchInput(newActive);
            }
            break;
    }

    switch (SWA_KoCalcIndex(ko.asap()))
    {
        case SWA_KoChSwitch:
            newActive = ko.value(DPT_Switch);
            logDebugP("SWA_KocSwitch: %u", newActive);

            processSwitchInput(newActive);
            break;
        case SWA_KoChLock:
            newActive = ko.value(DPT_Switch);
            logDebugP("SWA_KocLock: %u", newActive);

            processLockInput(newActive);
            break;
        case SWA_KoChScene:
            processScene((uint8_t)ko.value(Dpt(18, 1, 1)) + 1, ko.value(Dpt(18, 1, 0)));
            break;
    }

    logIndentDown();
}

void SwitchActuatorChannel::processSwitchInput(bool newActive)
{
    logDebugP("processSwitchInput (newActive=%u)", newActive);

    if (KoSWA_ChLockStatus.value(DPT_Switch))
    {
        logDebugP("Channel is locked");

        if (ParamSWA_ChBehaviorUnlock == 3)
        {
            statusDuringLock = newActive;
            logDebugP("statusDuringLock: %u", newActive);
        }

        return;
    }

    if (newActive == (bool)KoSWA_ChStatus.value(DPT_Switch))
    {
        logDebugP("New value equals current status");
        return;
    }

    doSwitch(newActive);
}

void SwitchActuatorChannel::processLockInput(bool newActive)
{
    if (newActive == (bool)KoSWA_ChLockStatus.value(DPT_Switch))
    {
        logDebugP("New value equals current status");
        return;
    }

    KoSWA_ChLockStatus.value(newActive, DPT_Switch);

    if (newActive)
    {
        statusDuringLock = (bool)KoSWA_ChStatus.value(DPT_Switch);

        if (ParamSWA_ChBehaviorLock > 0)
        {
            bool active = ParamSWA_ChBehaviorLock == 2;
            logDebugP("Lock active, switch relay (active=%u)", active);

            doSwitch(active);
        }
    }
    else
    {
        switch (ParamSWA_ChBehaviorUnlock)
        {
            case 1:
                logDebugP("Lock inative, switch relay (active=0)");
                doSwitch(false);
                break;
            case 2:
                logDebugP("Lock inative, switch relay (active=1)");
                doSwitch(true);
                break;
            case 3:
            case 4:
                logDebugP("Lock inative, switch relay (active=%u)", statusDuringLock);
                doSwitch(statusDuringLock);
                break;
        }
    }
}

void SwitchActuatorChannel::processScene(uint8_t sceneNumber, bool learn)
{
    logDebugP("processScene (sceneNumber=%u, learn=%u)", sceneNumber, learn);

    if (learn)
    {
        logInfoP("Scene learning not supported");
        return;
    }

    if (ParamSWA_ChSceneAActive &&
        ParamSWA_ChSceneANumber == sceneNumber)
    {
        switch (ParamSWA_ChSceneABehavior)
        {
            case 0:
                processSwitchInput(false);
                break;
            case 1:
                processSwitchInput(true);
                break;
            case 2:
                processLockInput(false);
                break;
            case 3:
                processLockInput(true);
                break;
        }
    }
    else if (ParamSWA_ChSceneBActive &&
             ParamSWA_ChSceneBNumber == sceneNumber)
    {
        switch (ParamSWA_ChSceneBBehavior)
        {
            case 0:
                processSwitchInput(false);
                break;
            case 1:
                processSwitchInput(true);
                break;
            case 2:
                processLockInput(false);
                break;
            case 3:
                processLockInput(true);
                break;
        }
    }
    else if (ParamSWA_ChSceneCActive &&
             ParamSWA_ChSceneCNumber == sceneNumber)
    {
        switch (ParamSWA_ChSceneCBehavior)
        {
            case 0:
                processSwitchInput(false);
                break;
            case 1:
                processSwitchInput(true);
                break;
            case 2:
                processLockInput(false);
                break;
            case 3:
                processLockInput(true);
                break;
        }
    }
    else if (ParamSWA_ChSceneDActive &&
             ParamSWA_ChSceneDNumber == sceneNumber)
    {
        switch (ParamSWA_ChSceneDBehavior)
        {
            case 0:
                processSwitchInput(false);
                break;
            case 1:
                processSwitchInput(true);
                break;
            case 2:
                processLockInput(false);
                break;
            case 3:
                processLockInput(true);
                break;
        }
    }
    else if (ParamSWA_ChSceneEActive &&
             ParamSWA_ChSceneENumber == sceneNumber)
    {
        switch (ParamSWA_ChSceneEBehavior)
        {
            case 0:
                processSwitchInput(false);
                break;
            case 1:
                processSwitchInput(true);
                break;
            case 2:
                processLockInput(false);
                break;
            case 3:
                processLockInput(true);
                break;
        }
    }
    else if (ParamSWA_ChSceneFActive &&
             ParamSWA_ChSceneFNumber == sceneNumber)
    {
        switch (ParamSWA_ChSceneFBehavior)
        {
            case 0:
                processSwitchInput(false);
                break;
            case 1:
                processSwitchInput(true);
                break;
            case 2:
                processLockInput(false);
                break;
            case 3:
                processLockInput(true);
                break;
        }
    }
    else if (ParamSWA_ChSceneGActive &&
             ParamSWA_ChSceneGNumber == sceneNumber)
    {
        switch (ParamSWA_ChSceneGBehavior)
        {
            case 0:
                processSwitchInput(false);
                break;
            case 1:
                processSwitchInput(true);
                break;
            case 2:
                processLockInput(false);
                break;
            case 3:
                processLockInput(true);
                break;
        }
    }
    else if (ParamSWA_ChSceneHActive &&
             ParamSWA_ChSceneHNumber == sceneNumber)
    {
        switch (ParamSWA_ChSceneHBehavior)
        {
            case 0:
                processSwitchInput(false);
                break;
            case 1:
                processSwitchInput(true);
                break;
            case 2:
                processLockInput(false);
                break;
            case 3:
                processLockInput(true);
                break;
        }
    }
}

void SwitchActuatorChannel::doSwitch(bool active, bool syncSwitch)
{
    logDebugP("doSwitch (active=%u, syncSwitch=%u)", active, syncSwitch);

    if (active && ParamSWA_ChTurnOnDelayTimeMS > 0)
    {
        turnOnDelayTimer = delayTimerInit();

        // if switch on during switch off delay, we won't turn off anymore
        turnOffDelayTimer = 0;
    }
    else if (!active && ParamSWA_ChTurnOffDelayTimeMS > 0)
    {
        turnOffDelayTimer = delayTimerInit();

        // if switch off during switch on delay, we won't turn on anymore
        turnOnDelayTimer = 0;
    }
    else
        doSwitchInternal(active, syncSwitch);
}

void SwitchActuatorChannel::doSwitchInternal(bool active, bool syncSwitch)
{
    if (ParamSWA_ChActive != 1)
    {
        logDebugP("doSwitchInternal: Channel not active (%u)", ParamSWA_ChActive);
        return;
    }

    if (_channelIndex >= OPENKNX_SWA_CHANNEL_COUNT)
    {
        logErrorP("Channel index %u is too high for hardware channel count %u", _channelIndex, OPENKNX_SWA_CHANNEL_COUNT);
        return;
    }

    // if current channel should be switch in sync with other channel
    // ignore current channel switch and switch main channel instead
    if (syncSwitch && ParamSWA_ChSyncSwitch == 1)
    {
        if ((_channelIndex) % 3 == 1)
            openknxSwitchActuatorModule.doSwitchChannel(_channelIndex - 1, active);
        else if ((_channelIndex) % 3 == 2)
            openknxSwitchActuatorModule.doSwitchChannel(_channelIndex - 2, active);

        return;
    }

    // ensure both coils are off first
    relaisOff();

    // invert in case of operation mode is opening contact
    bool activeSet = ParamSWA_ChOperationMode ? !active : active;
    if (activeSet)
    {
        logDebugP("Write relay state activeSet=%u to GPIO %u with value %u", activeSet, RELAY_SET_PINS[_channelIndex], OPENKNX_SWA_SET_ACTIVE_ON);
        openknx.gpio.digitalWrite(RELAY_SET_PINS[_channelIndex], OPENKNX_SWA_SET_ACTIVE_ON);
    }
    else
    {
        logDebugP("Write relay state activeSet=%u to GPIO %u with value %u", activeSet, RELAY_RESET_PINS[_channelIndex], OPENKNX_SWA_RESET_ACTIVE_ON);
        openknx.gpio.digitalWrite(RELAY_RESET_PINS[_channelIndex], OPENKNX_SWA_RESET_ACTIVE_ON);
    }
    relayBistableImpulsTimer = delayTimerInit();

    logDebugP("Set channel status %u", active);
    KoSWA_ChStatus.value(active, DPT_Switch);

    if (ParamSWA_ChStatusInverted)
    {
        logDebugP("Set channel inverted status %u", !active);
        KoSWA_ChStatusInverted.value(!active, DPT_Switch);
    }

    // every third channel can be a sync main channel the next two channels depend on if sync is enabled
    if (syncSwitch && ((_channelIndex) % 3 == 0))
    {
        uint8_t channelUp1Index = _channelIndex + 1;
        if (channelUp1Index < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT))
        {
            bool syncChannelUp1 = (bool)(knx.paramByte(SWA_ChSyncSwitch + SWA_ParamBlockOffset + channelUp1Index * SWA_ParamBlockSize) & SWA_ChSyncSwitchMask);
            if (syncChannelUp1)
                openknxSwitchActuatorModule.doSwitchChannel(channelUp1Index, active, false);
        }

        uint8_t channelUp2Index = _channelIndex + 2;
        if (channelUp2Index < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT))
        {
            bool syncChannelUp2 = (bool)(knx.paramByte(SWA_ChSyncSwitch + SWA_ParamBlockOffset + channelUp2Index * SWA_ParamBlockSize) & SWA_ChSyncSwitchMask);
            if (syncChannelUp2)
                openknxSwitchActuatorModule.doSwitchChannel(channelUp2Index, active, false);
        }
    }
}

void SwitchActuatorChannel::setup(bool configured)
{
    openknx.gpio.pinMode(RELAY_SET_PINS[_channelIndex], OUTPUT, true, !OPENKNX_SWA_SET_ACTIVE_ON);
    openknx.gpio.pinMode(RELAY_RESET_PINS[_channelIndex], OUTPUT, true, !OPENKNX_SWA_RESET_ACTIVE_ON);

    // set it again the standard way, just in case
    relaisOff();

    if (configured)
    {
        if (ParamSWA_ChStatusCyclicTimeMS > 0)
            statusCyclicSendTimer = delayTimerInit();

#ifdef OPENKNX_SWA_BL0942_SPI
        if (ParamSWA_ChMeasureActive)
            bl0942StartupDelay = delayTimerInit();
#endif
    }
}

void SwitchActuatorChannel::loop()
{
    if (relayBistableImpulsTimer > 0 && delayCheck(relayBistableImpulsTimer, OPENKNX_SWA_BISTABLE_IMPULSE_LENGTH))
    {
        relaisOff();
        relayBistableImpulsTimer = 0;
    }

    if (turnOnDelayTimer > 0 && delayCheck(turnOnDelayTimer, ParamSWA_ChTurnOnDelayTimeMS))
    {
        logDebugP("Turn relay on after turn on delay");
        doSwitchInternal(true);
        turnOnDelayTimer = 0;
    }
    if (turnOffDelayTimer > 0 && delayCheck(turnOffDelayTimer, ParamSWA_ChTurnOffDelayTimeMS))
    {
        logDebugP("Turn relay off after turn off delay");
        doSwitchInternal(false);
        turnOffDelayTimer = 0;
    }

    if (statusCyclicSendTimer > 0 && delayCheck(statusCyclicSendTimer, ParamSWA_ChStatusCyclicTimeMS))
    {
        KoSWA_ChStatus.objectWritten();
        statusCyclicSendTimer = delayTimerInit();
    }

#ifdef OPENKNX_SWA_SWITCH_PINS
    if (delayCheck(switchLastTrigger, SWITCH_DEBOUNCE) &&
        openknx.gpio.digitalRead(RELAY_SWITCH_PINS[_channelIndex]) == OPENKNX_SWA_SWITCH_ACTIVE_ON)
    {
        logDebugP("Button channel %u pressed", _channelIndex + 1);
        switchLastTrigger = delayTimerInit();
        doSwitch(!isRelayActive());
    }
#endif
#ifdef OPENKNX_SWA_STATUS_PINS
    openknx.gpio.digitalWrite(RELAY_STATUS_PINS[_channelIndex], isRelayActive() ? OPENKNX_SWA_STATUS_ACTIVE_ON : !OPENKNX_SWA_STATUS_ACTIVE_ON);
#endif

#ifdef OPENKNX_SWA_BL0942_SPI
    if (!bl0942Initialized &&
        bl0942StartupDelay > 0 && delayCheck(bl0942StartupDelay, OPENKNX_SWA_BL0942_INIT_DELAY))
    {
        openknx.gpio.pinMode(RELAY_MEASURE_EN_PINS[_channelIndex], OUTPUT, true, OPENKNX_SWA_MEASURE_EN_ACTIVE_ON);
        delay(10); // wait for BL0942 to start up

        setChannelSelectorBl0942(false);
        bl0942.setChannelSelector([this](bool active){
            this->setChannelSelectorBl0942(active);
        });
        bl0942.onDataReceived([this](bl0942::SensorData &data){
            this->dataReceivedBl0942(data);
        });

        initBl0942();

        if (ParamSWA_ChPowerSendCyclicTimeMS > 0)
            powerCyclicSendTimer = delayTimerInit();
        if (ParamSWA_ChCurrentSendCyclicTimeMS > 0)
            currentCyclicSendTimer = delayTimerInit();
        if (ParamSWA_ChVoltageSendCyclicTimeMS > 0)
            voltageCyclicSendTimer = delayTimerInit();
        
        bl0942Initialized = true;
    }

    if (bl0942Initialized)
    {
        if (delayCheck(bl0942UpdateTimer, OPENKNX_SWA_BL0942_LOOP_DELAY))
        {
            bl0942.loop();
            bl0942UpdateTimer = delayTimerInit();
        }

        if (ParamSWA_ChPowerSend &&
            powerCyclicSendTimer > 0 && delayCheck(powerCyclicSendTimer, ParamSWA_ChPowerSendCyclicTimeMS))
        {
            KoSWA_ChPower.value(lastPower, DPT_Value_Power);
            powerCyclicSendTimer = delayTimerInit();
        }

        if (ParamSWA_ChCurrentSend &&
            currentCyclicSendTimer > 0 && delayCheck(currentCyclicSendTimer, ParamSWA_ChCurrentSendCyclicTimeMS))
        {
            KoSWA_ChCurrent.value(lastCurrent * 1000.0f, DPT_UElCurrentmA);
            currentCyclicSendTimer = delayTimerInit();
        }

        if (ParamSWA_ChVoltageSend &&
            voltageCyclicSendTimer > 0 && delayCheck(voltageCyclicSendTimer, ParamSWA_ChVoltageSendCyclicTimeMS))
        {
            KoSWA_ChVoltage.value(lastVoltage * 1000.0f, DPT_Value_Volt);
            voltageCyclicSendTimer = delayTimerInit();
        }
    }
#endif
}

#ifdef OPENKNX_SWA_BL0942_SPI
void SwitchActuatorChannel::initBl0942()
{
    bl0942::ModeConfig config;

    // RMS refresh time (choose one)
    config.rms_update_freq = bl0942::UPDATE_FREQUENCY_400MS;  // or UPDATE_FREQUENCY_800MS

    // RMS waveform type
    config.rms_waveform = bl0942::RMS_WAVEFORM_FULL;          // or RMS_WAVEFORM_AC

    // Line frequency
    config.ac_freq = bl0942::LINE_FREQUENCY_50HZ;             // or LINE_FREQUENCY_60HZ

    // Clear energy counter on read
    //config.clear_mode = bl0942::CNT_CLR_SEL_ENABLE;           // or CNT_CLR_SEL_DISABLE
    config.clear_mode = bl0942::CNT_CLR_SEL_DISABLE;           // or CNT_CLR_SEL_DISABLE

    // Accumulation mode
    config.accumulation_mode = bl0942::ACCUMULATION_MODE_ABSOLUTE;  // or ALGEBRAIC

    bl0942.setup(config);
    //bl0942.print_registers();
}

void SwitchActuatorChannel::setChannelSelectorBl0942(bool active)
{
    if (active)
    {
        logTraceP("BL0942: selecting channel");
        openknx.gpio.digitalWrite(RELAY_MEASURE_CS_PINS[_channelIndex], OPENKNX_SWA_MEASURE_CS_ACTIVE_ON);
    }
    else
    {
        logTraceP("BL0942: unselecting channel");
        openknx.gpio.digitalWrite(RELAY_MEASURE_CS_PINS[_channelIndex], !OPENKNX_SWA_MEASURE_CS_ACTIVE_ON);
    }
}

void SwitchActuatorChannel::dataReceivedBl0942(bl0942::SensorData &data)
{
    if (_channelIndex == 0)
    {
        if (delayCheck(_debugTimer, 2000))
        {
            logDebugP("U: %.2f V, I: %.2f A, P: %.2f W", data.voltage, data.current, data.watt);
            _debugTimer = delayTimerInit();
        }
    }

    if (ParamSWA_ChPowerSend)
    {
        uint16_t powerDifference = round(abs(lastPower - data.watt));
        if (powerDifference > 0)
        {
            if (lastPower > 0 && powerDifference >= lastPower * ParamSWA_ChPowerSendMinChangePercent / 100.0f &&
                powerDifference >= ParamSWA_ChPowerSendMinChangeAbsolute)
                KoSWA_ChPower.value(data.watt, DPT_Value_Power);
            else
                KoSWA_ChPower.valueNoSend(data.watt, DPT_Value_Power);
        }
    }

    float newCurrent = data.current * 1000.0f;
    if (ParamSWA_ChCurrentSend)
    {
        uint16_t currentDifference = round(abs(lastCurrent - newCurrent));
        if (currentDifference > 0)
        {
            if (lastCurrent > 0 && currentDifference >= lastCurrent * ParamSWA_ChCurrentSendMinChangePercent / 100.0f &&
                currentDifference >= ParamSWA_ChCurrentSendMinChangeAbsolute)
                KoSWA_ChCurrent.value(newCurrent, DPT_UElCurrentmA);
            else
                KoSWA_ChCurrent.valueNoSend(newCurrent, DPT_UElCurrentmA);
        }
    }

    if (ParamSWA_ChVoltageSend)
    {
        uint16_t voltageDifference = round(abs(lastVoltage - data.voltage));
        if (voltageDifference > 0)
        {
            if (lastVoltage > 0 && voltageDifference >= lastVoltage * ParamSWA_ChVoltageSendMinChangePercent / 100.0f &&
                voltageDifference >= ParamSWA_ChVoltageSendMinChangeAbsolute)
                KoSWA_ChVoltage.value(data.voltage * 1000.0f, DPT_Value_Volt);
            else
                KoSWA_ChVoltage.valueNoSend(data.voltage * 1000.0f, DPT_Value_Volt);
        }
    }

    lastPower = data.watt;
    lastCurrent = newCurrent;
    lastVoltage = data.voltage;
}
#endif

void SwitchActuatorChannel::relaisOff()
{
    logDebugP("Write relay state off to GPIO %u with value %u", RELAY_SET_PINS[_channelIndex], !OPENKNX_SWA_SET_ACTIVE_ON);
    openknx.gpio.digitalWrite(RELAY_SET_PINS[_channelIndex], !OPENKNX_SWA_SET_ACTIVE_ON);

    logDebugP("Write relay state off to GPIO %u with value %u", RELAY_RESET_PINS[_channelIndex], !OPENKNX_SWA_RESET_ACTIVE_ON);
    openknx.gpio.digitalWrite(RELAY_RESET_PINS[_channelIndex], !OPENKNX_SWA_RESET_ACTIVE_ON);
}

bool SwitchActuatorChannel::isRelayActive()
{
    return (bool)KoSWA_ChStatus.value(DPT_Switch);
}

void SwitchActuatorChannel::savePower()
{
    if (ParamSWA_ChBehaviorPowerLoss > 0)
    {
        bool active = ParamSWA_ChBehaviorPowerLoss == 2;
        logDebugP("Save power, switch relay (active=%u)", active);

        doSwitchInternal(active);
    }
}

bool SwitchActuatorChannel::restorePower()
{
    if (ParamSWA_ChBehaviorPowerRegain > 0)
    {
        bool active = ParamSWA_ChBehaviorPowerRegain == 2;
        doSwitchInternal(active);

        logDebugP("Restore power, switch relay (active=%u)", active);
    }

    return true;
}

const std::string SwitchActuatorChannel::name()
{
    return "SwitchChannel";
}