#include "InputControl.h"

const InputControl::ActionFunc InputControl::_actionMap[] PROGMEM = {
    &InputControl::moveToNextStep, // 0
    &InputControl::startHeat,
    &InputControl::stopHeat,
    &InputControl::startFan,
    &InputControl::stopFan,
    &InputControl::startTurn, // 5
    &InputControl::stopTurn,
    &InputControl::startHumi,
    &InputControl::stopHumi,
    &InputControl::increaseValue,
    &InputControl::decreaseValue, // 8
    &InputControl::saveYes,
    &InputControl::saveNo,
    //&InputControl::autoTune,
    //&InputControl::autoTuneYes,
    //&InputControl::autoTuneNo
};

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
    &InputControl::increasePID_Kd};

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
    &InputControl::decreasePID_Kd};

void InputControl::calcBaseStep()
{
    uint32_t now = millis();
    if (now - _lastActionTime > 500)
    {
        _repeatCount = 0;
        _baseStep = 1;
    }
    else
    {
        _repeatCount++;
        _baseStep = 1 + (_repeatCount / 5) * (_repeatCount / 5);
        if (_baseStep > 100)
            _baseStep = 100; // 최대 제한
    }
    _lastActionTime = now;
}

#define CALL_MAP_FUNC(Map, Index, Type)                            \
    if (Index >= 0 && Index < (int)(sizeof(Map) / sizeof(Map[0]))) \
    {                                                              \
        Type handler;                                              \
        memcpy_P(&handler, &Map[Index], sizeof(Type));             \
        if (handler)                                               \
            (this->*handler)();                                    \
    }

void InputControl::handleAction(SystemAction action)
{
    int index = action - SystemAction::BUTTON_FIRST;
    CALL_MAP_FUNC(_actionMap, index, ActionFunc);
}

// --- VALUE_UP,   // 수치 증가 (UP Click) ---
void InputControl::increaseValue()
{
    calcBaseStep();
    int index = _view.getPageStep() - PageStep::SETUP_FIRST;
    CALL_MAP_FUNC(_increaseMap, index, IncreaseFunc);
}

// --- VALUE_DOWN, // 수치 감소 (DOWN Click) ---
void InputControl::decreaseValue()
{
    calcBaseStep();
    int index = _view.getPageStep() - PageStep::SETUP_FIRST;
    CALL_MAP_FUNC(_decreaseMap, index, DecreaseFunc);
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
    /*if (_operate.getAutoTune())
    {
        _operate.setAutoTune(true);
        return;
    }*/

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

#define DEFINE_MANUAL_CONTROL(Name, Flag)        \
    void InputControl::start##Name()             \
    {                                            \
        _operate.setManual##Name(true);          \
        _operate.setRelay##Name(true);           \
        _view.updateRelayFlag(UpdateFlag::Flag); \
    }                                            \
    void InputControl::stop##Name()              \
    {                                            \
        _operate.setManual##Name(false);         \
        _operate.setRelay##Name(false);          \
        _view.updateRelayFlag(UpdateFlag::Flag); \
    }

DEFINE_MANUAL_CONTROL(Heat, RELAY_HEAT)
DEFINE_MANUAL_CONTROL(Fan, RELAY_FAN)
DEFINE_MANUAL_CONTROL(Turn, RELAY_TURN)
DEFINE_MANUAL_CONTROL(Humi, RELAY_HUMI)

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

#define DEFINE_INC_DEC(Name, Getter, Setter, Min, Max, UseStep) \
void InputControl::increase##Name() { \
    uint32_t current = _view.Getter(); \
    uint16_t step = UseStep ? _baseStep : 1; \
    if (current < Max) { \
        uint32_t next = current + step; \
        _view.Setter(next > Max ? Max : (uint16_t)next); \
    } \
} \
void InputControl::decrease##Name() { \
    uint32_t current = _view.Getter(); \
    uint16_t step = UseStep ? _baseStep : 1; \
    if (current > (uint32_t)Min + step) _view.Setter(current - step); \
    else _view.Setter(Min); \
}

// TARGET_TEMP, TARGET_HUMI, TURNINTERVAL
DEFINE_INC_DEC(Temperature,  getTargetTempFixed, setTargetTempFixed, 200, 450,  false)
DEFINE_INC_DEC(Humidity,     getTargetHumiFixed, setTargetHumiFixed, 0,   999,  false)
DEFINE_INC_DEC(TurnInterval, getTurnInterval,    setTurnInterval,    0,   720,  true)
DEFINE_INC_DEC(TurnDuration, getTurnDuration,    setTurnDuration,    0,   99,   true)
DEFINE_INC_DEC(PID_Kp,       getPID_Kp,          setPID_Kp,          0,   32000, true)
DEFINE_INC_DEC(PID_Ki,       getPID_Ki,          setPID_Ki,          0,   32000, true)
DEFINE_INC_DEC(PID_Kd,       getPID_Kd,          setPID_Kd,          0,   32000, true)

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

/*void InputControl::autoTune()
{
    _operate.setAutoTuneWait(true);
    _view.updateRelayFlag(UpdateFlag::AUTOTUNE);
}

void InputControl::autoTuneYes()
{
    _operate.setAutoTuneWait(false);
    _operate.setAutoTune(true);
    _view.updateRelayFlag(UpdateFlag::AUTOTUNE);
}

void InputControl::autoTuneNo()
{
    _operate.setAutoTuneWait(false);
    _operate.setAutoTune(false);
    _view.updateRelayFlag(UpdateFlag::AUTOTUNE);
}*/