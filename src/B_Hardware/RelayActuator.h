#pragma once

#include <Arduino.h>
#include "A_Core/Interfaces.h"

// --- 실제 릴레이/액추에이터 제어 클래스 ---
class RelayActuator
{
public:
    template <size_t N>
    using Group = NormalGroup<RelayActuator, N>;

private:
    uint8_t _pin;
    bool _state = true;
    bool _output = true;
    bool _activeLow;

    // --- 시간 보호 관련 변수 추가 ---
    unsigned long _lastSwitchTime = 0; // 마지막으로 상태가 바뀐 시점
    unsigned long _minOnTime = 15000;   // 최소 켜짐 시간 (기본 15초)
    unsigned long _minOffTime = 30000;  // 최소 꺼짐 시간 (기본 30초)

public:
    RelayActuator(uint8_t pin, bool activeLow = true);
    virtual ~RelayActuator() {}

    void init();
    void update();
    void run();
    void stop();
    bool isRunning() const { return _state; }

    // 설정 변경이 필요할 경우를 위한 메서드
    void setMinTimes(unsigned long onTime, unsigned long offTime);
    void setOutput(bool output);
    bool canRun() const;
    bool canStop() const;
};
