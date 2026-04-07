#include "RelayActuator.h"

RelayActuator::RelayActuator(uint8_t pin, bool activeLow)
    : _pin(pin), _activeLow(activeLow) {}

void RelayActuator::init()
{
    pinMode(_pin, OUTPUT);
    stop(); // 초기 상태는 OFF
    _lastSwitchTime = millis() - _minOffTime; // 초기화 시점에서 바로 켤 수 있도록 설정
}

void RelayActuator::update()
{
    if (_output && canRun()) {
        run();
    } else if (!_output && canStop()) {
        stop();
    }
}

void RelayActuator::setMinTimes(unsigned long onTime, unsigned long offTime)
{
    _minOnTime = onTime;
    _minOffTime = offTime;
}

void RelayActuator::setOutput(bool output) {
    _output = output;
}

bool RelayActuator::canRun() const {
    return (millis() - _lastSwitchTime) >= _minOffTime;
}

bool RelayActuator::canStop() const {
    return (millis() - _lastSwitchTime) >= _minOnTime;
}

void RelayActuator::run()
{
    if (_state) 
        return;

    _state = true;
    _lastSwitchTime = millis(); // 상태 변화 시점 기록
    digitalWrite(_pin, _activeLow ? LOW : HIGH);
}

void RelayActuator::stop()
{
    if (!_state)
        return;

    _state = false;
    _lastSwitchTime = millis(); // 상태 변화 시점 기록
    digitalWrite(_pin, _activeLow ? HIGH : LOW);
}