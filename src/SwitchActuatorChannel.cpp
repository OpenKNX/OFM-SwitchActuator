#include "SwitchActuatorChannel.h"
#include "SwitchActuatorModule.h"

SwitchActuatorChannel::SwitchActuatorChannel(uint8_t iChannelNumber)
{
    _channelIndex = iChannelNumber;
}

SwitchActuatorChannel::~SwitchActuatorChannel() {}

void SwitchActuatorChannel::processInputKo(GroupObject &iKo)
{
    logDebugP("processInputKo: channel %u", _channelIndex);

    if (ParamSWA_ChannelActive != 1)
    {
        logDebugP("Channel not active (%u)", ParamSWA_ChannelActive);
        return;
    }
    
    bool newActive;
    switch (SWA_KoCalcIndex(iKo.asap()))
    {
        case SWA_KoCentralFunction:
            if (ParamSWA_ChannelCentralFunction)
            {
                newActive = iKo.value(DPT_Switch);
                logDebugP("SWA_KoCentralFunction: %u", newActive);

                processSwitchInput(newActive);
            }
            break;
        case SWA_KoChannelSwitch:
            newActive = iKo.value(DPT_Switch);
            logDebugP("SWA_KoChannelSwitch: %u", newActive);

            processSwitchInput(newActive);
            break;
        case SWA_KoChannelLock:
            newActive = iKo.value(DPT_Switch);
            logDebugP("SWA_KoChannelLock: %u", newActive);

            if (newActive == (bool)KoSWA_ChannelLockStatus.value(DPT_Switch))
            {
                logDebugP("New value equals current status");
                break;
            }

            KoSWA_ChannelLockStatus.value(newActive, DPT_Switch);

            if (newActive)
            {
                statusDuringLock = (bool)KoSWA_ChannelStatus.value(DPT_Switch);

                if (ParamSWA_ChannelBehaviorLock > 0)
                    doSwitch(ParamSWA_ChannelBehaviorLock == 2);
            }
            else
            {
                switch (ParamSWA_ChannelBehaviorUnlock)
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
}

void SwitchActuatorChannel::processSwitchInput(bool newActive)
{
    if (KoSWA_ChannelLockStatus.value(DPT_Switch))
    {
        logDebugP("Channel is locked");

        if (ParamSWA_ChannelBehaviorUnlock == 3)
        {
            statusDuringLock = newActive;
            logDebugP("statusDuringLock: %u", newActive);
        }

        return;
    }

    if (newActive == (bool)KoSWA_ChannelStatus.value(DPT_Switch))
    {
        logDebugP("New value equals current status");
        return;
    }

    doSwitch(newActive);
}

void SwitchActuatorChannel::doSwitch(bool active)
{
    if (_channelIndex >= OPENKNX_SWA_CHANNEL_COUNT)
    {
        logErrorP("Channel index %u is too high for hardware channel count %u", _channelIndex, OPENKNX_SWA_CHANNEL_COUNT);
        return;
    }

    // ensure both coils are off first
    relaisOff();

    // invert in case of operation mode is opening contact
    bool activeSet = ParamSWA_ChannelOperationMode ? !active : active;
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

    if (ParamSWA_ChannelStatusSend)
    {
        logDebugP("Set channel status %u", active);
        KoSWA_ChannelStatus.value(active, DPT_Switch);
    }
    else
        KoSWA_ChannelStatus.valueNoSend(active, DPT_Switch);

    if (ParamSWA_ChannelStatusInverted)
    {
        logDebugP("Set channel inverted status %u", !active);
        KoSWA_ChannelStatusInverted.value(!active, DPT_Switch);
    }
}

void SwitchActuatorChannel::loop()
{
    if (relayBistableImpulsTimer > 0 && delayCheck(relayBistableImpulsTimer, OPENKNX_SWA_BISTABLE_IMPULSE_LENGTH))
    {
        relaisOff();
        relayBistableImpulsTimer = 0;
    }
}

void SwitchActuatorChannel::setup()
{
    // preset PIN state before changing PIN mode
    digitalWriteFast(RELAY_SET_PINS[_channelIndex], RELAY_GPIO_SET_OFF);
    digitalWriteFast(RELAY_RESET_PINS[_channelIndex], RELAY_GPIO_RESET_OFF);

    pinMode(RELAY_SET_PINS[_channelIndex], OUTPUT);
    pinMode(RELAY_RESET_PINS[_channelIndex], OUTPUT);

    // set it again the standard way, just in case
    relaisOff();
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
    return (bool)KoSWA_ChannelStatus.value(DPT_Switch);
}

void SwitchActuatorChannel::savePower()
{
    if (ParamSWA_ChannelBehaviorPowerLoss > 0)
        doSwitch(ParamSWA_ChannelBehaviorPowerLoss == 2);
}

bool SwitchActuatorChannel::restorePower()
{
    if (ParamSWA_ChannelBehaviorPowerRegain > 0)
        doSwitch(ParamSWA_ChannelBehaviorPowerRegain == 2);
    
    return true;
}

const std::string SwitchActuatorChannel::name()
{
    return "SwitchChannel";
}