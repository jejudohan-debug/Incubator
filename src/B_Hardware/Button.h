#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "A_Core/Interfaces.h"
#include "B_Hardware/HardwarePins.h"

class Button : public IDevice
{
public:
public:
    template<size_t N>
    using Group = NormalGroup<Button, N>;

private:
    enum class InternalState : uint8_t
    {
        IDLE,       // 떼어진 상태
        DEBOUNCING, // 신호 변동 중 (안정화 대기)
        PRESSED,    // 눌린 상태 (안정화됨)
        // LONG_HELD   // 이미 롱 프레스 이벤트를 발생시킨 상태
    };

    uint8_t _pin;
    EventFlag::Type _id;
    InternalState _internalState = InternalState::IDLE;
    // DeviceEvent _latchedEvent = DeviceEvent::NONE;

    unsigned long _lastDebounceTime = 0;

    const unsigned long _debounceDelay = 50;
    // const unsigned long _longPressThreshold = 1500;

public:
    Button(uint8_t pin, EventFlag::Type id);
    void init() override;
    void update() override;
};
