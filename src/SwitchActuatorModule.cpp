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
    logDebugP("processInputKo");
    logIndentUp();

    switch (iKo.asap())
    {
        case SWA_KoCentralFunction:
            logDebugP("SWA_KoCentralFunction");
            // #ToDo
            break;
    }

    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
    {
        channel[i]->processInputKo(iKo);
    }

    logIndentDown();
}

void SwitchActuatorModule::loop()
{
    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
        channel[i]->loop();
}

void SwitchActuatorModule::setup()
{
    for (uint8_t i = 0; i < MIN(ParamSWA_VisibleChannels, OPENKNX_SWA_CHANNEL_COUNT); i++)
    {
        channel[i] = new SwitchActuatorChannel(i);
        channel[i]->setup();
    }
}