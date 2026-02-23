#pragma once

#include <Arduino.h>
#include "A_Core/CoreTypes.h"
#include "A_Core/Interfaces.h"

class Button{
private:
    uint8_t _pin;
    InternalState _internalState = InternalState::IDLE;
    ButtonEvent _latchedEvent = ButtonEvent::NONE;

    unsigned long _pressStartTime = 0;
    unsigned long _lastDebounceTime = 0;

    const unsigned long _debounceDelay = 50;
    const unsigned long _longPressThreshold = 1500;

public:
    Button(uint8_t pin);
    void init();
    void update();
    ButtonEvent pullEvent();
    bool isPressed() const;
};
