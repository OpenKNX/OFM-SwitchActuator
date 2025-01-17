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
#ifdef OPENKNX_GPIO_WIRE
    OPENKNX_GPIO_WIRE.setSDA(OPENKNX_GPIO_SDA);
    OPENKNX_GPIO_WIRE.setSCL(OPENKNX_GPIO_SCL);
    OPENKNX_GPIO_WIRE.begin();
    OPENKNX_GPIO_WIRE.setClock(OPENKNX_GPIO_CLOCK);
    
    if (tca.begin())
    {
        tca.pinMode8(0, 0x00);
        tca.pinMode8(1, 0xFF);
        tca.setPolarity8(1, 0xFF);

        for (uint8_t i = 0; i < 8; i++)
            tca.write1(i, LOW);

        logDebugP("TCA9555 setup done with address %u", tca.getAddress());
    }
    else
        logDebugP("TCA9555 not found at address %u", tca.getAddress());
#endif

    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
    {
        channel[i] = new SwitchActuatorChannel(i);
        channel[i]->setup(configured);
    }
}

void SwitchActuatorModule::loop()
{
    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
        channel[i]->loop();

#ifdef OPENKNX_GPIO_WIRE
    uint8_t channelIndex = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        channelIndex = 7 - i;
        if (delayCheck(chSwitchLastTrigger[channelIndex], CH_SWITCH_DEBOUNCE) && tca.read1(i + 8))
        {
            chSwitchLastTrigger[channelIndex] = delayTimerInit();
            channel[channelIndex]->doSwitch(!channel[channelIndex]->isRelayActive());
        }
    }

    for (uint8_t i = 0; i < 8; i++)
        tca.write1(i, channel[i]->isRelayActive());
#endif
}

void SwitchActuatorModule::doSwitchChannel(uint8_t channelIndex, bool active, bool syncSwitch)
{
    channel[channelIndex]->doSwitch(active, syncSwitch);
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