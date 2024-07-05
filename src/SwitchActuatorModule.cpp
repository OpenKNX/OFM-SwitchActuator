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
    return "SwitchActuator";
}

const std::string SwitchActuatorModule::version()
{
    return MODULE_SwitchActuatorModule_Version;
}

void SwitchActuatorModule::processInputKo(GroupObject &iKo)
{
    switch (iKo.asap())
    {
        case SWA_KoCentralFunction:
            // #ToDo
            break;
    }

    for (uint8_t i = 0; i < ParamSWA_VisibleChannels; i++)
    {
        channel[i]->processInputKo(iKo);
    }
}

void SwitchActuatorModule::loop()
{
}

void SwitchActuatorModule::setup()
{
    for (uint8_t i = 0; i < ParamSWA_VisibleChannels; i++)
    {
        channel[i] = new SwitchActuatorChannel(i);
        channel[i]->setup();
    }
}