#pragma once

#include <Arduino.h>
#include "A_Core/Interfaces.h"

// --- 실제 릴레이/액추에이터 제어 클래스 ---
class RelayActuator
{
public:
    template<size_t N>
    using Group = NormalGroup<RelayActuator, N>;

private:
    uint8_t _pin;
    bool _state = true;
    bool _activeLow;

public:
    RelayActuator(uint8_t pin, bool activeLow = true);
    virtual ~RelayActuator() {}

    void init();
    void update();
    void run();
    void stop();
    bool isRunning() const { return _state; }
};
