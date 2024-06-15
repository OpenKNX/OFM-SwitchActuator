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
}

void SwitchActuator::loop()
{
}

void SwitchActuator::setup()
{
    mNumChannels = ParamSWA_VisibleChannels; // knx.paramByte(SWA_PMChannels);
    for (uint8_t lIndex = 0; lIndex < mNumChannels; lIndex++)
    {
        mChannel[lIndex] = new SwitchActuatorChannel(lIndex);
        mChannel[lIndex]->setup();
    }
}