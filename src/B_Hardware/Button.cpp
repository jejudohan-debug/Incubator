#include "Button.h"

Button::Button(uint8_t pin, EventFlag::Type id)
    : _pin(pin), _id(id) {}

void Button::init()
{
    pinMode(_pin, INPUT_PULLUP);
}

void Button::update()
{
    bool isLogicPressed = (digitalRead(_pin) == LOW);
    unsigned long now = millis();

    switch (_internalState)
    {
    case InternalState::IDLE: // 버튼 눌릴 시 Pressed로
        if (isLogicPressed)
        {
            _internalState = InternalState::DEBOUNCING;
            _lastDebounceTime = now;
        }
        break;
    case InternalState::DEBOUNCING:
        if (now - _lastDebounceTime >= _debounceDelay)
        {
            if (isLogicPressed)
            {
                //_latchedEvent = DeviceEvent::PRESS;
                notify(_id, static_cast<uint8_t>(ButtonEvent::PRESS));
                _internalState = InternalState::PRESSED;
                _pressStartTime = now;
                // Serial.println(F("Button Pressed"));
            }
            else
            {
                _internalState = InternalState::IDLE;
            }
        }
        break;
    case InternalState::PRESSED:
        if (!isLogicPressed)
        {
            //_latchedEvent = DeviceEvent::CLICK;
            notify(_id, static_cast<uint8_t>(ButtonEvent::CLICK));
            _internalState = InternalState::IDLE;
            // Serial.println(F("Button Clicked"));
        }
        else if (now - _pressStartTime >= _longPressThreshold) {
            notify(_id, static_cast<uint8_t>(ButtonEvent::LONG_PRESS));
            _internalState = InternalState::LONG_HELD;
            _lastRepeatTime = now;
        }
        break;
    case InternalState::LONG_HELD:
        if (!isLogicPressed) {
            notify(_id, static_cast<uint8_t>(ButtonEvent::RELEASE));
            _internalState = InternalState::IDLE;
        }
        else {
            const unsigned long repeatInterval = 150; // 0.15초마다 숫자 변경
            if (now - _lastRepeatTime >= repeatInterval) {
                notify(_id, static_cast<uint8_t>(ButtonEvent::LONG_PRESS));
                _lastRepeatTime = now;
            }
        }
        break;
    }
}
