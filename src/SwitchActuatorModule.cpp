#include "SwitchActuatorModule.h"
#include "OpenKNX.h"
#include "ModuleVersionCheck.h"

SwitchActuatorModule openknxSwitchActuatorModule;

SwitchActuatorModule::SwitchActuatorModule()
{
}

SwitchActuatorModule::~SwitchActuatorModule()
{
}

const std::string SwitchActuatorModule::name()
{
    return "Switch";
}

const std::string SwitchActuatorModule::version()
{
    return MODULE_SwitchActuator_Version;
}

void SwitchActuatorModule::processInputKo(GroupObject &iKo)
{
    if (iKo.asap() != SWA_KoCentralFunction &&
        (iKo.asap() < SWA_KoBlockOffset ||
         iKo.asap() > SWA_KoBlockOffset + ParamSWA_VisibleChannels * SWA_KoBlockSize - 1))
        return;

    logDebugP("processInputKo");
    logIndentUp();

    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
        channel[i]->processInputKo(iKo);

    logIndentDown();
}

void SwitchActuatorModule::setup(bool configured)
{
#ifdef OPENKNX_SWA_STATUS_PINS
    for(int i = 0; i < OPENKNX_SWA_CHANNEL_COUNT; i++)
    {
        openknx.gpio.pinMode(RELAY_STATUS_PINS[i], OUTPUT, true, !OPENKNX_SWA_STATUS_ACTIVE_ON);
    }
#endif

#ifdef OPENKNX_SWA_SWITCH_PINS
    for(int i = 0; i < OPENKNX_SWA_CHANNEL_COUNT; i++)
    {
        openknx.gpio.pinMode(RELAY_SWITCH_PINS[i], INPUT);
    }
#endif

    // always setup all channels to ensure defined relay state
    for (uint8_t i = 0; i < OPENKNX_SWA_CHANNEL_COUNT; i++)
    {
        channel[i] = new SwitchActuatorChannel(i);
        channel[i]->setup(configured);
    }
}

void SwitchActuatorModule::loop()
{
    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
        channel[i]->loop();

#ifdef OPENKNX_SWA_SWITCH_PINS
    for (uint8_t channelIndex = 0; channelIndex < OPENKNX_SWA_CHANNEL_COUNT; channelIndex++)
    {
        if (delayCheck(chSwitchLastTrigger[channelIndex], CH_SWITCH_DEBOUNCE) &&
            openknx.gpio.digitalRead(RELAY_SWITCH_PINS[channelIndex]) == OPENKNX_SWA_SWITCH_ACTIVE_ON)
        {
            logDebugP("Button channel %u pressed", channelIndex + 1);
            chSwitchLastTrigger[channelIndex] = delayTimerInit();
            channel[channelIndex]->doSwitch(!channel[channelIndex]->isRelayActive());
        }
    }
#endif
#ifdef OPENKNX_SWA_STATUS_PINS
    for(int i = 0; i < OPENKNX_SWA_CHANNEL_COUNT; i++)
        openknx.gpio.digitalWrite(RELAY_STATUS_PINS[i], channel[i]->isRelayActive() ? OPENKNX_SWA_STATUS_ACTIVE_ON : !OPENKNX_SWA_STATUS_ACTIVE_ON);
#endif
}

void SwitchActuatorModule::doSwitchChannel(uint8_t channelIndex, bool active, bool syncSwitch)
{
    channel[channelIndex]->doSwitch(active, syncSwitch);
}

bool SwitchActuatorModule::getChannelStatus(uint8_t channelIndex)
{
    if (channelIndex >= OPENKNX_SWA_CHANNEL_COUNT)
        return false;

    return channel[channelIndex]->isRelayActive();
}

void SwitchActuatorModule::readFlash(const uint8_t *data, const uint16_t size)
{
    if (size == 0)
        return;

    logDebugP("Reading state from flash");
    logIndentUp();

    uint8_t version = openknx.flash.readByte();
    if (version != OPENKNX_SWA_FLASH_VERSION)
    {
        logDebugP("Invalid flash version %u", version);
        return;
    }

    uint32_t magicWord = openknx.flash.readInt();
    if (magicWord != OPENKNX_SWA_FLASH_MAGIC_WORD)
    {
        logDebugP("Flash content invalid");
        return;
    }

    uint8_t relayChannelsStored = openknx.flash.readByte();

    uint8_t byteValue = 0;
    for (uint8_t i = 0; i < MIN(relayChannelsStored, MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT)); i++)
    {
        uint8_t bitIndex = i % 8;
        if (bitIndex == 0)
            byteValue = openknx.flash.readByte();

        channel[i]->doSwitch(bitRead(byteValue, bitIndex), false);
    }
    
    logIndentDown();
}

void SwitchActuatorModule::writeFlash()
{
    openknx.flash.writeByte(OPENKNX_SWA_FLASH_VERSION);
    openknx.flash.writeInt(OPENKNX_SWA_FLASH_MAGIC_WORD);

    openknx.flash.writeByte(MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT));

    uint8_t byteValue = 0;
    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
    {
        uint8_t bitIndex = i % 8;
        if (bitIndex == 0)
        {
            if (i > 0)
                openknx.flash.writeByte(byteValue);

            byteValue = 0;
        }

        bitWrite(byteValue, bitIndex, channel[i]->isRelayActive());
    }
    openknx.flash.writeByte(byteValue);

    logDebugP("Relays state written to flash");
}

uint16_t SwitchActuatorModule::flashSize()
{
    return 6 + ceil(MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT) / 8.0);
}

void SwitchActuatorModule::savePower()
{
    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
        channel[i]->savePower();
}

bool SwitchActuatorModule::restorePower()
{
    bool success = true;
    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
        success &= channel[i]->restorePower();
    
    return success;
}

bool SwitchActuatorModule::processCommand(const std::string cmd, bool diagnoseKo)
{
    if (cmd.substr(0, 2) != "sa")
        return false;

    if (cmd.length() == 13 && cmd.substr(0, 10) == "sa switch ")
    {
        uint8_t channelidx = cmd.at(10) - 'a';
        uint8_t value = std::stoi(cmd.substr(12, 1));
        if(channelidx > OPENKNX_SWA_CHANNEL_COUNT - 1 || (value != 0 && value != 1))
        {
            logInfoP("wrong sytnax of command sa switch");
            return true;
        }
        
        logInfoP("Switch Channel %c to %d", channelidx+'a', value);
        chSwitchLastTrigger[channelidx] = delayTimerInit();
        channel[channelidx]->doSwitch(value);

        return true;
    }
    else if (cmd.length() == 11 && cmd.substr(0, 10) == "sa toggle ")
    {
        uint8_t channelidx = cmd.at(10) - 'a';
        if(channelidx > OPENKNX_SWA_CHANNEL_COUNT - 1)
        {
            logInfoP("wrong sytnax of command sa toggle");
            return true;
        }

        uint8_t value = !channel[channelidx]->isRelayActive();
        logInfoP("Switch Channel %c to %d", channelidx+'a', value);
        chSwitchLastTrigger[channelidx] = delayTimerInit();
        channel[channelidx]->doSwitch(value);

        return true;
    }

    // Commands starting with sa are our diagnose commands
    logInfoP("sa (SwitchActuator) command with bad args");
    if (diagnoseKo)
    {
        openknx.console.writeDiagenoseKo("sa: bad args");
    }
    return true;
}

void SwitchActuatorModule::showHelp()
{
    logInfo("sa switch <channel> 0-1", "set (1) / reset (0) channel a-%c", OPENKNX_SWA_CHANNEL_COUNT-1+'a');
    logInfo("sa toggle <channel>", "toggle channel a-%c", OPENKNX_SWA_CHANNEL_COUNT-1+'a');
}