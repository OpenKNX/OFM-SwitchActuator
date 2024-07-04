#include "SwitchActuator.h"
#include "OpenKNX.h"
#include "ModuleVersionCheck.h"

SwitchActuator openknxSwitchActuatorModule;

SwitchActuator::SwitchActuator()
{
}

SwitchActuator::~SwitchActuator()
{
}

const std::string SwitchActuator::name()
{
    return "SwitchActuator";
}

const std::string SwitchActuator::version()
{
    return MODULE_SwitchActuatorModule_Version;
}

void SwitchActuator::processInputKo(GroupObject &iKo)
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

void SwitchActuator::loop()
{
}

void SwitchActuator::setup()
{
    for (uint8_t i = 0; i < ParamSWA_VisibleChannels; i++)
    {
        channel[i] = new SwitchActuatorChannel(i);
        channel[i]->setup();
    }
}