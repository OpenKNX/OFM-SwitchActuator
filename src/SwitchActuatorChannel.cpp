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
#if OPENKNX_SWA_CHANNEL_COUNT > 0
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
#endif
}

void SwitchActuatorChannel::setup(bool configured)
{
#if OPENKNX_SWA_CHANNEL_COUNT > 0
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
#endif
}

void SwitchActuatorChannel::loop()
{
#if OPENKNX_SWA_CHANNEL_COUNT > 0
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
    if (ParamSWA_FrontControlInput)
    {
        if (delayCheck(switchLastTrigger, SWITCH_DEBOUNCE) &&
            openknx.gpio.digitalRead(RELAY_SWITCH_PINS[_channelIndex]) == OPENKNX_SWA_SWITCH_ACTIVE_ON)
        {
            logDebugP("Button channel %u pressed", _channelIndex + 1);
            switchLastTrigger = delayTimerInit();
            doSwitch(!isRelayActive());
        }
    }
#endif
#ifdef OPENKNX_SWA_STATUS_PINS
    if (ParamSWA_FrontControlOutput)
        openknx.gpio.digitalWrite(RELAY_STATUS_PINS[_channelIndex], isRelayActive() ? OPENKNX_SWA_STATUS_ACTIVE_ON : !OPENKNX_SWA_STATUS_ACTIVE_ON);
#endif

#ifdef OPENKNX_SWA_BL0942_SPI
    if (!bl0942Initialized &&
        bl0942StartupDelay > 0 && delayCheck(bl0942StartupDelay, OPENKNX_SWA_BL0942_INIT_DELAY))
    {
        openknx.gpio.pinMode(RELAY_MEASURE_EN_PINS[_channelIndex], OUTPUT, true, OPENKNX_SWA_MEASURE_EN_ACTIVE_ON);
        delay(10); // wait for BL0942 to start up

        setChannelSelectorBl0942(false);
        bl0942.setChannelSelector([this](bool active) {
            this->setChannelSelectorBl0942(active);
        });

        initBl0942();

        if (ParamSWA_ChPowerSendCyclicTimeMS > 0)
            powerCyclicSendTimer = delayTimerInit();
        if (ParamSWA_ChCurrentSendCyclicTimeMS > 0)
            currentCyclicSendTimer = delayTimerInit();
        if (ParamSWA_ChVoltageSendCyclicTimeMS > 0)
            voltageCyclicSendTimer = delayTimerInit();
        
        bl0942Initialized = true;
        
        Serial.println("dump all registers in HEX\n");

        Serial.print("BL0942_REG_I_WAVE:\t");
        Serial.println(bl0942.readRegister(0x01), HEX);
        Serial.print("BL0942_REG_V_WAVE:\t");
        Serial.println(bl0942.readRegister(0x02), HEX);
        Serial.print("BL0942_REG_I_RMS:\t");
        Serial.println(bl0942.readRegister(0x03), HEX);
        Serial.print("BL0942_REG_V_RMS:\t");
        Serial.println(bl0942.readRegister(0x04), HEX);
        Serial.print("BL0942_REG_I_FAST_RMS:\t");
        Serial.println(bl0942.readRegister(0x05), HEX);
        Serial.print("BL0942_REG_WATT:\t");
        Serial.println(bl0942.readRegister(0x06), HEX);
        Serial.print("BL0942_REG_CF_CNT:\t");
        Serial.println(bl0942.readRegister(0x07), HEX);
        Serial.print("BL0942_REG_FREQ:\t");
        Serial.println(bl0942.readRegister(0x08), HEX);
        Serial.print("BL0942_REG_STATUS:\t");
        Serial.println(bl0942.readRegister(0x09), HEX);
        Serial.print("BL0942_REG_I_RMSOS:\t");
        Serial.println(bl0942.readRegister(0x12), HEX);
        Serial.print("BL0942_REG_WA_CREEP:\t");
        Serial.println(bl0942.readRegister(0x14), HEX);
        Serial.print("BL0942_REG_I_FAST_RMS_TH:\t");
        Serial.println(bl0942.readRegister(0x15), HEX);
        Serial.print("BL0942_REG_I_FAST_RMS_CYC:\t");
        Serial.println(bl0942.readRegister(0x16), HEX);
        Serial.print("BL0942_REG_FREQ_CYC:\t");
        Serial.println(bl0942.readRegister(0x17), HEX);
        Serial.print("BL0942_REG_OT_FUNX:\t");
        Serial.println(bl0942.readRegister(0x18), HEX);
        Serial.print("BL0942_REG_MODE:\t");
        Serial.println(bl0942.readRegister(0x19), HEX);
        Serial.print("BL0942_REG_GAIN_CR:\t");
        Serial.println(bl0942.readRegister(0x1A), HEX);
        Serial.print("BL0942_REG_SOFT_RESET:\t");
        Serial.println(bl0942.readRegister(0x1C), HEX);
        Serial.print("BL0942_REG_USR_WRPROT:\t");
        Serial.println(bl0942.readRegister(0x1D), HEX);
    }

    if (bl0942Initialized)
    {
        if (delayCheck(bl0942UpdateTimer, OPENKNX_SWA_BL0942_LOOP_DELAY))
        {
            float voltage = bl0942.getVRMS();
            float current = bl0942.getIRMS();
            float watt = bl0942.getWatt();

            if (_channelIndex == 0)
            {
                if (delayCheck(_debugTimer, 2000))
                {
                    logDebugP("U: %.2f V, I: %.2f A, P: %.2f W", voltage, current, watt);
                    _debugTimer = delayTimerInit();
                }
            }

            if (OPENKNX_SWA_BL0942_INVERT_DIRECTION)
                watt = -watt;

            if (watt < 0)
                current *= -1;

            lastReceivedPower = watt;
            lastReceivedCurrent = current;
            lastReceivedVoltage = voltage;

            if (ParamSWA_ChPowerSend)
            {
                uint16_t powerDifference = round(abs(lastSentPower - watt));
                if (powerDifference > 0)
                {
                    if (lastSentPower > 0 && powerDifference >= lastSentPower * ParamSWA_ChPowerSendMinChangePercent / 100.0f &&
                        powerDifference >= ParamSWA_ChPowerSendMinChangeAbsolute)
                    {
                        KoSWA_ChPower.value(watt, DPT_Value_Power);
                        lastSentPower = watt;
                    }
                    else
                        KoSWA_ChPower.valueNoSend(watt, DPT_Value_Power);
                }
            }

            if (ParamSWA_ChCurrentSend)
            {
                uint16_t currentDifference = round(abs(lastSentCurrent - current * 1000));
                if (currentDifference > 0)
                {
                    if (lastSentCurrent > 0 && currentDifference >= lastSentCurrent * ParamSWA_ChCurrentSendMinChangePercent / 100.0f &&
                        currentDifference >= ParamSWA_ChCurrentSendMinChangeAbsolute)
                    {
                        KoSWA_ChCurrent.value(current, DPT_Value_Electric_Current);
                        lastSentCurrent = current * 1000;
                    }
                    else
                        KoSWA_ChCurrent.valueNoSend(current, DPT_Value_Electric_Current);
                }
            }

            if (ParamSWA_ChVoltageSend)
            {
                uint16_t voltageDifference = round(abs(lastSentVoltage - voltage));
                if (voltageDifference > 0)
                {
                    if (lastSentVoltage > 0 && voltageDifference >= lastSentVoltage * ParamSWA_ChVoltageSendMinChangePercent / 100.0f &&
                        voltageDifference >= ParamSWA_ChVoltageSendMinChangeAbsolute)
                    {
                        KoSWA_ChVoltage.value(voltage, DPT_Value_Electric_Potential);
                        lastSentVoltage = voltage;
                    }
                    else
                        KoSWA_ChVoltage.valueNoSend(voltage, DPT_Value_Electric_Potential);
                }
            }
            bl0942UpdateTimer = delayTimerInit();
        }

        if (ParamSWA_ChPowerSend &&
            powerCyclicSendTimer > 0 && delayCheck(powerCyclicSendTimer, ParamSWA_ChPowerSendCyclicTimeMS))
        {
            KoSWA_ChPower.value(lastReceivedPower, DPT_Value_Power);
            lastSentPower = lastReceivedPower;
            powerCyclicSendTimer = delayTimerInit();
        }

        if (ParamSWA_ChCurrentSend &&
            currentCyclicSendTimer > 0 && delayCheck(currentCyclicSendTimer, ParamSWA_ChCurrentSendCyclicTimeMS))
        {
            KoSWA_ChCurrent.value(lastReceivedCurrent, DPT_Value_Electric_Current);
            lastSentCurrent = lastReceivedCurrent * 1000;
            currentCyclicSendTimer = delayTimerInit();
        }

        if (ParamSWA_ChVoltageSend &&
            voltageCyclicSendTimer > 0 && delayCheck(voltageCyclicSendTimer, ParamSWA_ChVoltageSendCyclicTimeMS))
        {
            KoSWA_ChVoltage.value(lastReceivedVoltage, DPT_Value_Electric_Potential);
            lastSentVoltage = lastReceivedVoltage;
            voltageCyclicSendTimer = delayTimerInit();
        }
    }
#endif
#endif
}

#ifdef OPENKNX_SWA_BL0942_SPI
void SwitchActuatorChannel::initBl0942()
{
    // bl0942::ModeConfig config;

    // // RMS refresh time (choose one)
    // config.rms_update_freq = bl0942::UPDATE_FREQUENCY_400MS;  // or UPDATE_FREQUENCY_800MS

    // // RMS waveform type
    // config.rms_waveform = bl0942::RMS_WAVEFORM_FULL;          // or RMS_WAVEFORM_AC

    // // Line frequency
    // config.ac_freq = bl0942::LINE_FREQUENCY_50HZ;             // or LINE_FREQUENCY_60HZ

    // // Clear energy counter on read
    // //config.clear_mode = bl0942::CNT_CLR_SEL_ENABLE;           // or CNT_CLR_SEL_DISABLE
    // config.clear_mode = bl0942::CNT_CLR_SEL_DISABLE;           // or CNT_CLR_SEL_DISABLE

    // // Accumulation mode
    // config.accumulation_mode = bl0942::ACCUMULATION_MODE_ABSOLUTE;  // or ALGEBRAIC

    // bl0942.setup(config);

    bl0942.begin();
    bl0942.calibrate(0.001f);

    uint16_t mode =
        BL0942_MODE_RMS_UPDATE_400MS |
        BL0942_MODE_RMS_WAVEFORM_FULL |
        BL0942_MODE_AC_FREQUENCY_50HZ |
        BL0942_MODE_ACCU_MODE_ABSOLUTE;
    bl0942.setUserMode(mode);
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

float SwitchActuatorChannel::getPower()
{
    return lastReceivedPower;
}

float SwitchActuatorChannel::getCurrent()
{
    return lastReceivedCurrent;
}
#endif

void SwitchActuatorChannel::relaisOff()
{
#if OPENKNX_SWA_CHANNEL_COUNT > 0
    logDebugP("Write relay state off to GPIO %u with value %u", RELAY_SET_PINS[_channelIndex], !OPENKNX_SWA_SET_ACTIVE_ON);
    openknx.gpio.digitalWrite(RELAY_SET_PINS[_channelIndex], !OPENKNX_SWA_SET_ACTIVE_ON);

    logDebugP("Write relay state off to GPIO %u with value %u", RELAY_RESET_PINS[_channelIndex], !OPENKNX_SWA_RESET_ACTIVE_ON);
    openknx.gpio.digitalWrite(RELAY_RESET_PINS[_channelIndex], !OPENKNX_SWA_RESET_ACTIVE_ON);
#endif
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