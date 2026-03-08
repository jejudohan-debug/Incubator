#include "RelayActuator.h"

RelayActuator::RelayActuator(uint8_t pin, bool activeLow)
    : _pin(pin), _activeLow(activeLow) {}

void RelayActuator::init()
{
    pinMode(_pin, OUTPUT);
    stop(); // 초기 상태는 OFF
}

void RelayActuator::update()
{
    // 물리적인 핀 제어는 run/stop에서 수행하거나 여기서 주기적으로 확인 가능
}

void RelayActuator::run()
{
    if (_state)
        return;

    _state = true;
    digitalWrite(_pin, _activeLow ? LOW : HIGH);
}

void RelayActuator::stop()
{
    if (!_state)
        return;

    _state = false;
    digitalWrite(_pin, _activeLow ? HIGH : LOW);
}