#include "SwitchActuatorChannel.h"
#include "SwitchActuatorModule.h"

SwitchActuatorChannel::SwitchActuatorChannel(uint8_t iChannelNumber)
{
}

SwitchActuatorChannel::~SwitchActuatorChannel() {}

void SwitchActuatorChannel::processInputKo(GroupObject &iKo)
{
    if (ParamSWA_ChannelActive != 1)
        return;
    
    bool newActive;
    switch (SWA_KoCalcIndex(iKo.asap()))
    {
        case SWA_KoChannelSwitch:
            newActive = iKo.value(DPT_Switch);

            if (KoSWA_ChannelLockStatus.value(DPT_Switch))
            {
                if (ParamSWA_ChannelBehaviorUnlock == 3)
                    statusDuringLock = newActive;

                break;
            }

            if (newActive == (bool)KoSWA_ChannelStatus.value(DPT_Switch))
                break;

            doSwitch(newActive);
            break;
        case SWA_KoChannelLock:
            newActive = iKo.value(DPT_Switch);
            if (newActive == (bool)KoSWA_ChannelLockStatus.value(DPT_Switch))
                break;

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

void SwitchActuatorChannel::doSwitch(bool active)
{
    if (_channelIndex >= OPENKNX_SWA_CHANNEL_COUNT)
    {
        logErrorP("Channel index %u is too high for hardware channel count %u", _channelIndex, OPENKNX_SWA_CHANNEL_COUNT);
        return;
    }

    // invert in case of operation mode is opening contact
    bool activeSet = ParamSWA_ChannelOperationMode ? !active : active;
    if (activeSet)
        digitalWrite(RELAY_SET_PINS[_channelIndex], OPENKNX_SWA_SET_ACTIVE_ON == HIGH ? true : false);
    else
        digitalWrite(RELAY_RESET_PINS[_channelIndex], OPENKNX_SWA_RESET_ACTIVE_ON == HIGH ? true : false);

    if (ParamSWA_ChannelStatusSend)
        KoSWA_ChannelStatus.value(active, DPT_Switch);
    else
        KoSWA_ChannelStatus.valueNoSend(active, DPT_Switch);

    if (ParamSWA_ChannelStatusInverted)
        KoSWA_ChannelStatusInverted.value(!active, DPT_Switch);
}

void SwitchActuatorChannel::loop()
{
}

void SwitchActuatorChannel::setup()
{
}

const std::string SwitchActuatorChannel::name()
{
    return "SWA";
}