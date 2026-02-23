#include "SetupControl.h"

void SetupControl::init() {}

void SetupControl::update() {}

void SetupControl::handleAction(SystemAction action)
{
    if (action >= SystemAction::END_SETUP && action <= SystemAction::SAVE_NO)
    {
        int index = static_cast<int>(action) - static_cast<int>(SystemAction::END_SETUP);
        const int mapSize = sizeof(_setupActionMap) / sizeof(_setupActionMap[0]);
        if (index >= 0 && index < mapSize)
        {
            ActionHandler handler = _setupActionMap[index];
            if (handler)
            {
                (this->*handler)();
            }
        }
    }
}

void SetupControl::increaseValue()
{
    int index = static_cast<int>(_view.setup.step);
    const int mapSize = sizeof(_increaseHandlerMap) / sizeof(_increaseHandlerMap[0]);
    if (index >= 0 && index < mapSize)
    {
        IncreaseHandler handler = _increaseHandlerMap[index];
        if (handler)
        {
            (this->*handler)();
        }
    }
}

void SetupControl::decreaseValue()
{
    int index = static_cast<int>(_view.setup.step);
    const int mapSize = sizeof(_decreaseHandlerMap) / sizeof(_decreaseHandlerMap[0]);
    if (index >= 0 && index < mapSize)
    {
        DecreaseHandler handler = _decreaseHandlerMap[index];
        if (handler)
        {
            (this->*handler)();
        }
    }
}

void SetupControl::endSetupMode()
{
    _view.setup.isWaitingConfirm = false;
    _view.mode = SystemModes::NORMAL;
    _view.isUpdated = true;
}

void SetupControl::moveToNextStep()
{
    // 값이 변경되었으면 확인 화면만 표시
    if (_config != _tempConfig)
    {
        _view.setup.isWaitingConfirm = true;
        _view.isUpdated = true;
        return;
    }

    // 값이 변경되지 않았으면 스텝 이동
    int next = static_cast<int>(_view.setup.step) + 1;
    if (next >= static_cast<int>(SetupStep::COUNT))
    {
        next = 0;
    }
    _view.setup.step = static_cast<SetupStep>(next);
    _view.isUpdated = true;
}

void SetupControl::waitConfirm()
{
    if (_config != _tempConfig)
    {
        _view.setup.isWaitingConfirm = true;
    }
    else
    {
        _view.setup.isWaitingConfirm = false;
    }
    _view.isUpdated = true;
}

void SetupControl::confirmSave()
{
    if (_config != _tempConfig)
    {
        _config = _tempConfig;
    }
    _view.setup.isWaitingConfirm = false;

    // 다음 스텝으로 이동
    int next = static_cast<int>(_view.setup.step) + 1;
    if (next >= static_cast<int>(SetupStep::COUNT))
    {
        next = 0;
    }
    _view.setup.step = static_cast<SetupStep>(next);
    _view.isUpdated = true;
}

void SetupControl::cancelSave()
{
    // 변경 사항 취소
    _tempConfig = _config;
    _view.setup.isWaitingConfirm = false;

    // 다음 스텝으로 이동
    int next = static_cast<int>(_view.setup.step) + 1;
    if (next >= static_cast<int>(SetupStep::COUNT))
    {
        next = 0;
    }
    _view.setup.step = static_cast<SetupStep>(next);
    _view.isUpdated = true;
}

// --- increase handlers ---
void SetupControl::increaseSpecies()
{
    if (_view.species < static_cast<Species>(static_cast<int>(Species::COUNT) - 1))
    {
        _tempConfig.selectedSpecies = static_cast<uint8_t>(_view.species) + 1;
        _view.species = static_cast<Species>(_tempConfig.selectedSpecies);
    }
    _view.isUpdated = true;
}

void SetupControl::increaseDay()
{
    if (_view.d < 99)
    {
        _view.d++;
        _tempConfig.incubationStartTime -= 24UL * 60 * 60;
    }
    _view.isUpdated = true;
}

void SetupControl::increaseHour()
{
    if (_view.h < 23)
    {
        _view.h++;
        _tempConfig.incubationStartTime -= 60 * 60;
    }
    _view.isUpdated = true;
}

void SetupControl::increaseMinute()
{
    if (_view.m < 59)
    {
        _view.m++;
        _tempConfig.incubationStartTime -= 60;
    }
    _view.isUpdated = true;
}

void SetupControl::increaseTemperature()
{
    if (_view.setup.targetTemp < 45.0f)
    {
        _view.setup.targetTemp += 0.5f;
        _tempConfig.targetTemp = _view.setup.targetTemp;
    }
    _view.isUpdated = true;
}

void SetupControl::increaseHumidity()
{
    if (_view.setup.targetHumi < 100.0f)
    {
        _view.setup.targetHumi += 1.0f;
        _tempConfig.targetHumid = _view.setup.targetHumi;
    }
    _view.isUpdated = true;
}

void SetupControl::increaseTurnInterval()
{
    if (_view.setup.turnInterval < 720)
    {
        _view.setup.turnInterval++;
        _tempConfig.turnInterval = _view.setup.turnInterval;
    }
    _view.isUpdated = true;
}

// --- decrease handlers ---
void SetupControl::decreaseSpecies()
{
    if (static_cast<int>(_view.species) > 0)
    {
        _tempConfig.selectedSpecies = static_cast<uint8_t>(_view.species) - 1;
        _view.species = static_cast<Species>(_tempConfig.selectedSpecies);
    }
    _view.isUpdated = true;
}

void SetupControl::decreaseDay()
{
    if (_view.d > 0)
    {
        _view.d--;
        _tempConfig.incubationStartTime += 24UL * 60 * 60;
    }
    _view.isUpdated = true;
}

void SetupControl::decreaseHour()
{
    if (_view.h > 0)
    {
        _view.h--;
        _tempConfig.incubationStartTime += 60 * 60;
    }
    _view.isUpdated = true;
}

void SetupControl::decreaseMinute()
{
    if (_view.m > 0)
    {
        _view.m--;
        _tempConfig.incubationStartTime += 60;
    }
    _view.isUpdated = true;
}

void SetupControl::decreaseTemperature()
{
    if (_view.setup.targetTemp > 10.0f)
    {
        _view.setup.targetTemp -= 0.5f;
        _tempConfig.targetTemp = _view.setup.targetTemp;
    }
    _view.isUpdated = true;
}

void SetupControl::decreaseHumidity()
{
    if (_view.setup.targetHumi > 0.0f)
    {
        _view.setup.targetHumi -= 1.0f;
        _tempConfig.targetHumid = _view.setup.targetHumi;
    }
    _view.isUpdated = true;
}

void SetupControl::decreaseTurnInterval()
{
    if (_view.setup.turnInterval > 0)
    {
        _view.setup.turnInterval--;
        _tempConfig.turnInterval = _view.setup.turnInterval;
    }
    _view.isUpdated = true;
}
