#include "DisplayState.h"

#define MAKE_SETTER_GETTER(Name, Type, Var, Flag) \
    void DisplayState::set##Name(Type val)        \
    {                                             \
        updateField(Var, val, Flag);              \
    }                                             \
    Type DisplayState::get##Name() const          \
    {                                             \
        return Var;                               \
    }

void DisplayState::uint16ToString(char *buf, uint16_t value100)
{
    uint16_t value = (value100 + 5) / 10; // 소수점 첫째 자리까지 표현하기 위해 10으로 나눔
    if (value > 999)
        value = 999;

    uint16_t integral = value / 10;
    uint8_t fractional = value % 10;

    buf[0] = (integral / 10) + '0';
    buf[1] = (integral % 10) + '0';
    buf[2] = '.';
    buf[3] = fractional + '0';
    buf[4] = '\0';
}

template <typename T>
void DisplayState::updateField(T &field, T newValue, UpdateFlag::Type flag)
{
    if (field != newValue)
    {
        field = newValue;
        updateFlags |= flag;
    }
}

template <typename T>
void DisplayState::updateFieldClamped(T &field, T newValue, T minVal, T maxVal, UpdateFlag::Type flag)
{
    if (newValue < minVal)
        newValue = minVal;
    if (newValue > maxVal)
        newValue = maxVal;
    updateField(field, newValue, flag);
}

// --- setter, getter ---
void DisplayState::updateRelayFlag(UpdateFlag::Type flag)
{
    updateFlags |= flag;
}

bool DisplayState::getRelayFlag(UpdateFlag::Type flag)
{
    return UpdateFlag::hasFlag(updateFlags, flag);
}

MAKE_SETTER_GETTER(CurrentTempFixed, uint16_t, _currentTemp, UpdateFlag::CURRENT_VALUE)
MAKE_SETTER_GETTER(CurrentHumiFixed, uint16_t, _currentHumi, UpdateFlag::CURRENT_VALUE)
MAKE_SETTER_GETTER(TargetTempFixed,  uint16_t, _targetTemp,  UpdateFlag::TARGET_VALUE)
MAKE_SETTER_GETTER(TargetHumiFixed,  uint16_t, _targetHumi,  UpdateFlag::TARGET_VALUE)

MAKE_SETTER_GETTER(TurnInterval, uint16_t, _turnInterval, UpdateFlag::TARGET_VALUE)
MAKE_SETTER_GETTER(TurnDuration, uint16_t, _turnDuration, UpdateFlag::TARGET_VALUE)

MAKE_SETTER_GETTER(PID_Kp, uint16_t, _PID_Kp, UpdateFlag::PID_GAIN)
MAKE_SETTER_GETTER(PID_Ki, uint16_t, _PID_Ki, UpdateFlag::PID_GAIN)
MAKE_SETTER_GETTER(PID_Kd, uint16_t, _PID_Kd, UpdateFlag::PID_GAIN)


void DisplayState::setPageStep(PageStep s)
{
    updateField(_pageStep, s, UpdateFlag::ALL);
}

PageStep DisplayState::getPageStep() const
{
    return _pageStep;
}

void DisplayState::setSpecies(Species s)
{
    if (s < Species::COUNT)
    {
        updateField(_species, s, UpdateFlag::SPECIES);
    }
}

Species DisplayState::getSpecies() const
{
    return _species;
}

void DisplayState::calculateElapsedTime()
{
    if (_currentUnixTime < _startUnixTime)
    {
        _d = _h = _m = 0;
        updateFlags |= UpdateFlag::TIME;
        return;
    }

    uint32_t diff = _currentUnixTime - _startUnixTime;
    uint32_t totalMin = diff / 60;
    static uint32_t lastTotalMin = 0xFFFFFFFF;

    if (totalMin != lastTotalMin)
    {
        _d = diff / 86400;
        _h = (diff % 86400) / 3600;
        _m = (diff % 3600) / 60;

        lastTotalMin = totalMin;
        updateFlags |= UpdateFlag::TIME;
    }
}

void DisplayState::setCurrentUnixTime(uint32_t val)
{
    if (_currentUnixTime == val)
        return;

    _currentUnixTime = val;
    updateFlags |= UpdateFlag::TIME;
    calculateElapsedTime();
}

uint32_t DisplayState::getCurrentUnixTime()
{
    return _currentUnixTime;
}
void DisplayState::setStartUnixTime(uint32_t t)
{
    if (_startUnixTime == t)
        return;

    _startUnixTime = t;
    // updateField(_startUnixTime, t, UpdateFlag::TIME);
    calculateElapsedTime();
}

uint32_t DisplayState::getStartUnixTime() const
{
    return _startUnixTime;
}

uint32_t DisplayState::getElapsedUnixTime() const
{
    return _currentUnixTime - _startUnixTime;
}

uint8_t DisplayState::getElapsedDay() const
{
    return _d;
}

uint8_t DisplayState::getElapsedHour() const
{
    return _h;
}

uint8_t DisplayState::getElapsedMinute() const
{
    return _m;
}

void DisplayState::setAutotuneCycle(int val)
{
    _autoTuneCycle = val;
}

int DisplayState::getAutotuneCycle() const
{
    return _autoTuneCycle;
}