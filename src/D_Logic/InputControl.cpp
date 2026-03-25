#include "InputControl.h"

const InputControl::ActionFunc InputControl::_actionMap[] PROGMEM = {
    &InputControl::moveToNextStep, // 0
    &InputControl::startHeat,
    &InputControl::stopHeat,
    &InputControl::startFan,
    &InputControl::stopFan,
    &InputControl::startTurn, // 5
    &InputControl::stopTurn,
    &InputControl::increaseValue,
    &InputControl::decreaseValue, // 8
    &InputControl::saveYes,
    &InputControl::saveNo};

const InputControl::IncreaseFunc InputControl::_increaseMap[] PROGMEM = {
    &InputControl::increaseSpecies,
    &InputControl::increaseDay,
    &InputControl::increaseHour,
    &InputControl::increaseMinute,
    &InputControl::increaseTemperature,
    &InputControl::increaseHumidity,
    &InputControl::increaseTurnInterval,
    &InputControl::increaseTurnDuration,
    &InputControl::increasePID_Kp,
    &InputControl::increasePID_Ki,
    &InputControl::increasePID_Kd
};

const InputControl::DecreaseFunc InputControl::_decreaseMap[] PROGMEM = {
    &InputControl::decreaseSpecies,
    &InputControl::decreaseDay,
    &InputControl::decreaseHour,
    &InputControl::decreaseMinute,
    &InputControl::decreaseTemperature,
    &InputControl::decreaseHumidity,
    &InputControl::decreaseTurnInterval,
    &InputControl::decreaseTurnDuration,
    &InputControl::decreasePID_Kp,
    &InputControl::decreasePID_Ki,
    &InputControl::decreasePID_Kd
};

void InputControl::handleAction(SystemAction action)
{
    int index = action - SystemAction::BUTTON_FIRST;
    const int mapSize = sizeof(_actionMap) / sizeof(_actionMap[0]);

    if (index >= 0 && index < mapSize)
    {
        ActionFunc handler;
        memcpy_P(&handler, &_actionMap[index], sizeof(ActionFunc));
        if (handler != nullptr)
        {
            (this->*handler)();
        }
    }
}

// --- VALUE_UP,   // 수치 증가 (UP Click) ---
void InputControl::increaseValue()
{
    int index = _view.getPageStep() - PageStep::SETUP_FIRST;
    const int mapSize = sizeof(_increaseMap) / sizeof(_increaseMap[0]);
    if (index >= 0 && index < mapSize)
    {
        IncreaseFunc handler;
        memcpy_P(&handler, &_increaseMap[index], sizeof(IncreaseFunc));
        if (handler != nullptr)
        {
            (this->*handler)();
        }
    }
}

// --- VALUE_DOWN, // 수치 감소 (DOWN Click) ---
void InputControl::decreaseValue()
{
    int index = _view.getPageStep() - PageStep::SETUP_FIRST;
    const int mapSize = sizeof(_decreaseMap) / sizeof(_decreaseMap[0]);
    if (index >= 0 && index < mapSize)
    {
        DecreaseFunc handler;
        memcpy_P(&handler, &_decreaseMap[index], sizeof(DecreaseFunc));
        if (handler != nullptr)
        {
            (this->*handler)();
        }
    }
}

// MOVE_NEXT,  // 다음 단계 (SELECT Click)
void InputControl::moveToNextStep()
{
    // 값이 변경되었으면 확인 화면 표시
    if (_cfgEEPROM.isViewChanged())
    {
        _operate.setWaiting(true);
        return;
    }

    // 값이 변경되지 않았으면 스텝 이동
    int next = static_cast<int>(_view.getPageStep()) + 1;
    if (next >= static_cast<int>(PageStep::COUNT))
        next = 0;

    _view.setPageStep(static_cast<PageStep>(next));
    //_view.updateFlags |= UpdateFlag::STEP;
}

// SAVE_YES,   // 저장 확인: 예
void InputControl::saveYes()
{
    if (_cfgEEPROM.getSpecies() != _view.getSpecies())
    {
        _cfgEEPROM.resetSpeciesConfig(_view.getSpecies());
    }
    else
    {
        _cfgEEPROM.importViewConfigValue();
    }
    _operate.setWaiting(false);
    moveToNextStep();
}

// SAVE_NO,    // 저장 확인: 아니오
void InputControl::saveNo()
{
    _cfgEEPROM.exportViewConfigValue();

    _operate.setWaiting(false);
    moveToNextStep();
}

void InputControl::startHeat()
{
    _operate.setManualHeat(true);
    _view.updateRelayFlag(UpdateFlag::RELAY_HEAT);
}

// HEATER_STOP,  // 모터 수동 정지
void InputControl::stopHeat()
{
    _operate.setManualHeat(false);
    _view.updateRelayFlag(UpdateFlag::RELAY_HEAT);
}

// FAN_START,
void InputControl::startFan()
{
    _operate.setManualFan(true);
    _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
}

// FAN_STOP,
void InputControl::stopFan()
{
    _operate.setManualFan(false);
    _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
}

// TURN_START,
void InputControl::startTurn()
{
    _operate.setManualTurn(true);
    _view.updateRelayFlag(UpdateFlag::RELAY_TURN);
}

// TURN_STOP,
void InputControl::stopTurn()
{
    _operate.setManualTurn(false);
    _view.updateRelayFlag(UpdateFlag::RELAY_TURN);
}

// SPECIES,
void InputControl::updateSelectedSpecies(Species newSpecies)
{
    _view.setSpecies(newSpecies);
    _view.updateFlags |= UpdateFlag::SPECIES;
}

void InputControl::increaseSpecies()
{
    uint8_t current = static_cast<uint8_t>(_view.getSpecies());
    if (current < (static_cast<uint8_t>(Species::COUNT) - 1))
    {
        updateSelectedSpecies(static_cast<Species>(current + 1));
    }
}

void InputControl::decreaseSpecies()
{
    uint8_t current = static_cast<uint8_t>(_view.getSpecies());
    if (current > 0)
    {
        updateSelectedSpecies(static_cast<Species>(current - 1));
    }
}

// DAY, HOUR, MINUTE
void InputControl::modifyStartTime(uint32_t seconds, bool bPlus)
{
    uint32_t tStart = _view.getStartUnixTime();
    if (bPlus)
        tStart -= seconds;
    else
        tStart += seconds;

    _view.setStartUnixTime(tStart);
    //_view.updateFlags |= UpdateFlag::TIME; // |= 연산자 사용 필수!
}

void InputControl::increaseDay()
{
    if (_view.getElapsedDay() < 99)
    {
        modifyStartTime(24UL * 3600, true);
    }
}

void InputControl::decreaseDay()
{
    if (_view.getElapsedDay() > 0)
    {
        modifyStartTime(24UL * 3600, false);
    }
}

void InputControl::increaseHour()
{
    if (_view.getElapsedHour() < 23)
    {
        modifyStartTime(3600, true);
    }
}

void InputControl::decreaseHour()
{
    if (_view.getElapsedHour() > 0)
    {
        modifyStartTime(3600, false);
    }
}

void InputControl::increaseMinute()
{
    if (_view.getElapsedMinute() < 59)
    {
        modifyStartTime(60, true);
    }
}

void InputControl::decreaseMinute()
{
    if (_view.getElapsedMinute() > 0)
    {
        modifyStartTime(60, false);
    }
}

// TARGET_TEMP, TARGET_HUMI, TURNINTERVAL
void InputControl::increaseTemperature()
{
    uint16_t next = _view.getTargetTempFixed() + 2;
    if (next <= 450)
        _view.setTargetTemp(next);
}

void InputControl::decreaseTemperature()
{
    uint16_t next = _view.getTargetTempFixed() - 2;
    if (next > 200)
        _view.setTargetTemp(next);
}

void InputControl::increaseHumidity()
{
    uint16_t next = _view.getTargetHumiFixed() + 10;
    if (next <= 999)
        _view.setTargetHumi(next);
}

void InputControl::decreaseHumidity()
{
    uint16_t next = _view.getTargetHumiFixed() - 10;
    if (next > 0)
        _view.setTargetHumi(next);
}

void InputControl::increaseTurnInterval()
{
    uint16_t current = _view.getTurnInterval();
    const uint16_t MAX_VAL = 720;

    if (current < MAX_VAL)
    {
        uint16_t step = (current < 10) ? 1 : 10;
        uint16_t next = current + step;

        _view.setTurnInterval(next > MAX_VAL ? MAX_VAL : next);
    }
}

void InputControl::decreaseTurnInterval()
{
    uint16_t current = _view.getTurnInterval();
    const uint16_t MIN_VAL = 0;

    if (current > MIN_VAL)
    {
        uint16_t step = (current <= 10) ? 1 : 10;
        if (current > step)
        {
            uint16_t next = current - step;
            _view.setTurnInterval(next < MIN_VAL ? MIN_VAL : next);
        }
        else
        {
            _view.setTurnInterval(MIN_VAL);
        }
    }
}

void InputControl::increaseTurnDuration()
{
    uint16_t next = _view.getTurnDuration() + 1;
    if (next < 100)
    {
        _view.setTurnDuration(next);
    }
}

void InputControl::decreaseTurnDuration()
{
    uint16_t next = _view.getTurnDuration() - 1;
    if (next > 0) // 12시간 * 60분
    {
        _view.setTurnDuration(next);
    }
}

void InputControl::increasePID_Kp()
{
    int16_t current = _view.getPID_Kp();
    if (current < 999) 
    {
        _view.setPID_Kp(current + 1);
    }
}

void InputControl::decreasePID_Kp()
{
    int16_t current = _view.getPID_Kp();
    if (current > 0)
    {
        _view.setPID_Kp(current - 1);
    }
}

void InputControl::increasePID_Ki()
{
    int16_t current = _view.getPID_Ki();
    if (current < 999) 
    {
        _view.setPID_Ki(current + 1);
    }
}

void InputControl::decreasePID_Ki()
{
    int16_t current = _view.getPID_Ki();
    if (current > 0)
    {
        _view.setPID_Ki(current - 1);
    }
}

void InputControl::increasePID_Kd()
{
    int16_t current = _view.getPID_Kd();
    if (current < 999) 
    {
        _view.setPID_Kd(current + 1);
    }
}

void InputControl::decreasePID_Kd()
{
    int16_t current = _view.getPID_Kd();
    if (current > 0)
    {
        _view.setPID_Kd(current - 1);
    }
}


/* / CONFIRM,    // 확인
void InputControl::waitConfirm()
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
}*/
