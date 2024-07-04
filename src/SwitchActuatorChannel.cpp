#include "SwitchActuatorChannel.h"
#include "SwitchActuator.h"

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
    // invert in case of operation mode is opening contact
    bool activeSet = ParamSWA_ChannelOperationMode ? !active : active;

    if (activeSet)
    {
        uint8_t pinNumber;
        switch (_channelIndex)
        {
            case 0:
                pinNumber = RELAY_CH0_SET;
                break;
            case 1:
                pinNumber = RELAY_CH1_SET;
                break;
            case 2:
                pinNumber = RELAY_CH2_SET;
                break;
            case 3:
                pinNumber = RELAY_CH3_SET;
                break;
            case 4:
                pinNumber = RELAY_CH4_SET;
                break;
            case 5:
                pinNumber = RELAY_CH5_SET;
                break;
            case 6:
                pinNumber = RELAY_CH6_SET;
                break;
            case 7:
                pinNumber = RELAY_CH7_SET;
                break;
            default:
                logErrorP("PIN number for channel %u unknown", _channelIndex);
                return;
        }

        digitalWrite(pinNumber, RELAY_SET_ACTIVE_ON == HIGH ? true : false);
    }
    else
    {
        uint8_t pinNumber;
        switch (_channelIndex)
        {
            case 0:
                pinNumber = RELAY_CH0_RESET;
                break;
            case 1:
                pinNumber = RELAY_CH1_RESET;
                break;
            case 2:
                pinNumber = RELAY_CH2_RESET;
                break;
            case 3:
                pinNumber = RELAY_CH3_RESET;
                break;
            case 4:
                pinNumber = RELAY_CH4_RESET;
                break;
            case 5:
                pinNumber = RELAY_CH5_RESET;
                break;
            case 6:
                pinNumber = RELAY_CH6_RESET;
                break;
            case 7:
                pinNumber = RELAY_CH7_RESET;
                break;
            default:
                logErrorP("PIN number for channel %u unknown", _channelIndex);
                return;
        }

        digitalWrite(pinNumber, RELAY_RESET_ACTIVE_ON == HIGH ? true : false);
    }

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