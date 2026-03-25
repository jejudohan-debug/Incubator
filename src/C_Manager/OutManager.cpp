#include "OutputManager.h"

void OutputManager::init()
{
    for (auto &relay : _relays)
    {
        relay.init();
    }
    _ssr.init();
}

/*void OutputManager::switchingRelay(int index, UpdateFlag::Type flag, bool isOn)
{
    if (!UpdateFlag::hasFlag(_view.updateFlag, flag)) return;
}*/

void OutputManager::update()
{
    for (const auto &map : _relayMaps)
    {
        if (UpdateFlag::hasFlag(_view.updateFlags, map.flag))
        {
            bool isOn = (_operate.*(map.getter))();

            isOn ? _relays[map.index].run() : _relays[map.index].stop();
        }
    }

    if (UpdateFlag::hasFlag(_view.updateFlags, UpdateFlag::RELAY_HEAT))
    {
        _ssr.setOutput(computeIntegerPID());
    }
    _ssr.update();
}

int16_t OutputManager::computeIntegerPID()
{
    //int32_t error = (int32_t)target - (int32_t)current;
    int32_t error = (int32_t)_view.getTargetTempFixed() - _view.getCurrentTempFixed();

    // 1. P항
    int32_t pTerm = (int32_t)_view.getPID_Kp() * error;

    static int16_t lastOutput = 0;
    if (lastOutput < 255 || error < 0)
    {
        integral += error;
    }
    integral = constrain(integral, -1000, 1000); 
    int32_t iTerm = (int32_t)_view.getPID_Ki() * integral;

    // 3. D항
    int32_t dTerm = (int32_t)_view.getPID_Kd() * (error - lastError);
    
    // 4. 합산 및 스케일 조정 (100으로 나누어 정수화)
    int32_t total = (pTerm + iTerm + dTerm + 50) / 100;

    lastOutput = (int16_t)constrain(total, 0, 255);
    lastError = error;

    return lastOutput;
}
