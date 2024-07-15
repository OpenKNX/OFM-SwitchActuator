#include "SwitchActuatorChannel.h"
#include "SwitchActuatorModule.h"

SwitchActuatorChannel::SwitchActuatorChannel(uint8_t iChannelNumber)
{
    _channelIndex = iChannelNumber;
}

SwitchActuatorChannel::~SwitchActuatorChannel() {}

void SwitchActuatorChannel::processInputKo(GroupObject &iKo)
{
    if (ParamSWA_cActive != 1)
    {
        logTraceP("processInputKo: channel not active (%u)", ParamSWA_cActive);
        return;
    }

    logDebugP("processInputKo: channel %u", _channelIndex);
    logIndentUp();

    bool newActive;
    switch (iKo.asap())
    {
        case SWA_KoCentralFunction:
            if (ParamSWA_cCentralFunction)
            {
                newActive = iKo.value(DPT_Switch);
                logDebugP("SWA_KoCentralFunction: %u", newActive);

                processSwitchInput(newActive);
            }
            break;
    }

    switch (SWA_KoCalcIndex(iKo.asap()))
    {
        case SWA_KocSwitch:
            newActive = iKo.value(DPT_Switch);
            logDebugP("SWA_KocSwitch: %u", newActive);

            processSwitchInput(newActive);
            break;
        case SWA_KocLock:
            newActive = iKo.value(DPT_Switch);
            logDebugP("SWA_KocLock: %u", newActive);

            if (newActive == (bool)KoSWA_cLockStatus.value(DPT_Switch))
            {
                logDebugP("New value equals current status");
                break;
            }

            KoSWA_cLockStatus.value(newActive, DPT_Switch);

            if (newActive)
            {
                statusDuringLock = (bool)KoSWA_cStatus.value(DPT_Switch);

                if (ParamSWA_cBehaviorLock > 0)
                    doSwitch(ParamSWA_cBehaviorLock == 2);
            }
            else
            {
                switch (ParamSWA_cBehaviorUnlock)
                {
                    case 1:
                        doSwitch(false);
                        break;
                    case 2:
                        doSwitch(true);
                        break;
                    case 3:
                    case 4:
                        doSwitch(statusDuringLock);
                        break;
                }
            }

            break;
    }

    logIndentDown();
}

void SwitchActuatorChannel::processSwitchInput(bool newActive)
{
    if (KoSWA_cLockStatus.value(DPT_Switch))
    {
        logDebugP("Channel is locked");

        if (ParamSWA_cBehaviorUnlock == 3)
        {
            statusDuringLock = newActive;
            logDebugP("statusDuringLock: %u", newActive);
        }

        return;
    }

    if (newActive == (bool)KoSWA_cStatus.value(DPT_Switch))
    {
        logDebugP("New value equals current status");
        return;
    }

    doSwitch(newActive);
}

void SwitchActuatorChannel::doSwitch(bool active, bool syncSwitch)
{
    if (active && ParamSWA_cTurnOnDelayTimeMS > 0)
    {
        turnOnDelayTimer = delayTimerInit();

        // if switch on during switch off delay, we won't turn off anymore
        turnOffDelayTimer = 0;
    }
    else if (!active && ParamSWA_cTurnOffDelayTimeMS > 0)
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
    if (ParamSWA_cActive != 1)
    {
        logDebugP("Channel not active (%u)", ParamSWA_cActive);
        return;
    }

    if (_channelIndex >= OPENKNX_SWA_CHANNEL_COUNT)
    {
        logErrorP("Channel index %u is too high for hardware channel count %u", _channelIndex, OPENKNX_SWA_CHANNEL_COUNT);
        return;
    }

    // if current channel should be switch in sync with other channel
    // ignore current channel switch and switch main channel instead
    if (syncSwitch && ParamSWA_cSyncSwitch == 1)
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
    bool activeSet = ParamSWA_cOperationMode ? !active : active;
    if (activeSet)
    {
        logDebugP("Write relay state activeSet=%u to GPIO %u with value %u", activeSet, RELAY_SET_PINS[_channelIndex], RELAY_GPIO_SET_ON);
        digitalWrite(RELAY_SET_PINS[_channelIndex], RELAY_GPIO_SET_ON);
    }
    else
    {
        logDebugP("Write relay state activeSet=%u to GPIO %u with value %u", activeSet, RELAY_RESET_PINS[_channelIndex], RELAY_GPIO_RESET_ON);
        digitalWrite(RELAY_RESET_PINS[_channelIndex], RELAY_GPIO_RESET_ON);
    }
    relayBistableImpulsTimer = delayTimerInit();

    if (ParamSWA_cStatusSend)
    {
        logDebugP("Set channel status %u", active);
        KoSWA_cStatus.value(active, DPT_Switch);
    }
    else
        KoSWA_cStatus.valueNoSend(active, DPT_Switch);

    if (ParamSWA_cStatusInverted)
    {
        logDebugP("Set channel inverted status %u", !active);
        KoSWA_cStatusInverted.value(!active, DPT_Switch);
    }

    // every third channel can be a sync main channel the next two channels depend on if sync is enabled
    if (syncSwitch && ((_channelIndex) % 3 == 0))
    {
        uint8_t channelUp1Index = _channelIndex + 1;
        if (channelUp1Index < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT))
        {
            bool syncChannelUp1 = (bool)(knx.paramByte(SWA_cSyncSwitch + SWA_ParamBlockOffset + channelUp1Index * SWA_ParamBlockSize) & SWA_cSyncSwitchMask);
            if (syncChannelUp1)
                openknxSwitchActuatorModule.doSwitchChannel(channelUp1Index, active, false);
        }

        uint8_t channelUp2Index = _channelIndex + 2;
        if (channelUp2Index < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT))
        {
            bool syncChannelUp2 = (bool)(knx.paramByte(SWA_cSyncSwitch + SWA_ParamBlockOffset + channelUp2Index * SWA_ParamBlockSize) & SWA_cSyncSwitchMask);
            if (syncChannelUp2)
                openknxSwitchActuatorModule.doSwitchChannel(channelUp2Index, active, false);
        }
    }
}

void SwitchActuatorChannel::loop()
{
    if (relayBistableImpulsTimer > 0 && delayCheck(relayBistableImpulsTimer, OPENKNX_SWA_BISTABLE_IMPULSE_LENGTH))
    {
        relaisOff();
        relayBistableImpulsTimer = 0;
    }

    if (turnOnDelayTimer > 0 && delayCheck(turnOnDelayTimer, ParamSWA_cTurnOnDelayTimeMS))
    {
        doSwitchInternal(true);
        turnOnDelayTimer = 0;
    }
    if (turnOffDelayTimer > 0 && delayCheck(turnOffDelayTimer, ParamSWA_cTurnOffDelayTimeMS))
    {
        doSwitchInternal(false);
        turnOffDelayTimer = 0;
    }

    if (statusCyclicSendTimer > 0 && delayCheck(statusCyclicSendTimer, ParamSWA_cStatusCyclicTimeMS))
    {
        KoSWA_cStatus.objectWritten();
        statusCyclicSendTimer = delayTimerInit();
    }
}

void SwitchActuatorChannel::setup(bool configured)
{
    // preset PIN state before changing PIN mode
    digitalWriteFast(RELAY_SET_PINS[_channelIndex], RELAY_GPIO_SET_OFF);
    digitalWriteFast(RELAY_RESET_PINS[_channelIndex], RELAY_GPIO_RESET_OFF);

    pinMode(RELAY_SET_PINS[_channelIndex], OUTPUT);
    pinMode(RELAY_RESET_PINS[_channelIndex], OUTPUT);

    // set it again the standard way, just in case
    relaisOff();

    if (configured)
    {
        if (ParamSWA_cStatusCyclicTimeMS > 0)
            statusCyclicSendTimer = delayTimerInit();
    }
}

void SwitchActuatorChannel::relaisOff()
{
    logDebugP("Write relay state off to GPIO %u with value %u", RELAY_SET_PINS[_channelIndex], RELAY_GPIO_SET_OFF);
    digitalWrite(RELAY_SET_PINS[_channelIndex], RELAY_GPIO_SET_OFF);

    logDebugP("Write relay state off to GPIO %u with value %u", RELAY_RESET_PINS[_channelIndex], RELAY_GPIO_RESET_OFF);
    digitalWrite(RELAY_RESET_PINS[_channelIndex], RELAY_GPIO_RESET_OFF);
}

bool SwitchActuatorChannel::isRelayActive()
{
    return (bool)KoSWA_cStatus.value(DPT_Switch);
}

void SwitchActuatorChannel::savePower()
{
    if (ParamSWA_cBehaviorPowerLoss > 0)
        doSwitchInternal(ParamSWA_cBehaviorPowerLoss == 2);
}

bool SwitchActuatorChannel::restorePower()
{
    if (ParamSWA_cBehaviorPowerRegain > 0)
        doSwitchInternal(ParamSWA_cBehaviorPowerRegain == 2);

    return true;
}

const std::string SwitchActuatorChannel::name()
{
    return "SwitchChannel";
}