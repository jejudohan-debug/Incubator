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
    }
}
