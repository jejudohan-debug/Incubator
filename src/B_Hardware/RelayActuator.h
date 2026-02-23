#pragma once

#include <Arduino.h>
#include "A_Core/Interfaces.h"

// --- 실제 릴레이/액추에이터 제어 클래스 ---
class RelayActuator : public IActuator {
private:
    uint8_t _pin;
    bool _state = false;
    const char* _name;
    bool _activeLow;

public:
    RelayActuator(uint8_t pin, const char* name, bool activeLow = false) 
        : _pin(pin), _name(name), _activeLow(activeLow) {}

    void init() override {
        pinMode(_pin, OUTPUT);
        stop(); // 초기 상태는 OFF
    }

    void update() override {
        // 물리적인 핀 제어는 run/stop에서 수행하거나 여기서 주기적으로 확인 가능
    }

    void run() override {
        _state = true;
        digitalWrite(_pin, _activeLow ? LOW : HIGH);
    }

    void stop() override {
        _state = false;
        digitalWrite(_pin, _activeLow ? HIGH : LOW);
    }

    bool isRunning() const override {
        return _state;
    }

    virtual ~RelayActuator() {}
};
