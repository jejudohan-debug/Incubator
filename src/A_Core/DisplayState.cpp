#include "DisplayState.h"

void DisplayState::uint16ToString(char *buf, uint16_t value)
{
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

// --- setter, getter ---
void DisplayState::updateRelayFlag(UpdateFlag::Type flag)
{
    updateFlags |= flag;
}

bool DisplayState::getRelayFlag(UpdateFlag::Type flag)
{
    return UpdateFlag::hasFlag(updateFlags, flag);
}

void DisplayState::setCurrentTemp(float val)
{
    setCurrentTemp(floatToFixed(val));
}

void DisplayState::setCurrentTemp(uint16_t val)
{
    updateField(_currentTemp, val, UpdateFlag::CURRENT_VALUE);
}

float DisplayState::getCurrentTempFloat() const
{
    return fixedToFloat(_currentTemp);
}

uint16_t DisplayState::getCurrentTempFixed() const
{
    return _currentTemp;
}

void DisplayState::setCurrentHumi(float val)
{
    setCurrentHumi(floatToFixed(val));
}

void DisplayState::setCurrentHumi(uint16_t val)
{
    updateField(_currentHumi, val, UpdateFlag::CURRENT_VALUE);
}

float DisplayState::getCurrentHumiFloat() const
{
    return fixedToFloat(_currentHumi);
}

uint16_t DisplayState::getCurrentHumiFixed() const
{
    return _currentHumi;
}

void DisplayState::setTargetTemp(float val)
{
    setTargetTemp(floatToFixed(val));
}

void DisplayState::setTargetTemp(uint16_t val)
{
    updateField(_targetTemp, val, UpdateFlag::TARGET_VALUE);
}

float DisplayState::getTargetTempFloat() const
{
    return fixedToFloat(_targetTemp);
}

uint16_t DisplayState::getTargetTempFixed() const
{
    return _targetTemp;
}

void DisplayState::setTargetHumi(float val)
{
    setTargetHumi(floatToFixed(val));
}

void DisplayState::setTargetHumi(uint16_t val)
{
    updateField(_targetHumi, val, UpdateFlag::TARGET_VALUE);
}

float DisplayState::getTargetHumiFloat() const
{
    return fixedToFloat(_targetHumi);
}

uint16_t DisplayState::getTargetHumiFixed() const
{
    return _targetHumi;
}

void DisplayState::setTurnInterval(uint16_t val)
{
    updateField(_turnInterval, val, UpdateFlag::TARGET_VALUE);
}

uint16_t DisplayState::getTurnInterval() const
{
    return _turnInterval;
}

void DisplayState::setTurnDuration(uint16_t val)
{
    updateField(_turnDuration, val, UpdateFlag::TARGET_VALUE);
}

uint16_t DisplayState::getTurnDuration() const
{
    return _turnDuration;
}

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
    // updateField(_currentUnixTime, val, UpdateFlag::TIME);
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

/*void DisplayState::importConfigValue(const SystemConfig &cfg)
{
    _targetTemp = cfg.targetTemp;
    _targetHumi = cfg.targetHumi;
    _turnInterval = cfg.turnInterval;
    _turnDuration = cfg.turnDuration;
    _species = static_cast<Species>(cfg.selectedSpecies);
    _startUnixTime = cfg.incubationStartTime;
}*/

/* void SystemManager::onSpeciesChanged(Species newSpecies) {
    auto& state = SystemContext::getInstance().getView();

    // 1. 상태 객체의 종 정보를 먼저 변경
    state.setSpecies(newSpecies);

    // 2. 종에 따른 목표 온도/습도 자동 설정 (Manager의 역할)
    switch(newSpecies) {
    }
}*/