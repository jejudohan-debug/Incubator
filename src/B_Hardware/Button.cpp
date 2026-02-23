#include "Button.h"

Button::Button(uint8_t pin) : _pin(pin) {}

void Button::init(){
    pinMode(_pin, INPUT_PULLUP);
}

void Button::update(){
    bool isLogicPressed = (digitalRead(_pin) == LOW);
    unsigned long now = millis();

    switch (_internalState) {
        case InternalState::IDLE:   // 버튼 눌릴 시 Pressed로
            if (isLogicPressed) {
                _internalState = InternalState::DEBOUNCING;
                _lastDebounceTime = now;
            }
            break;
        case InternalState::DEBOUNCING:
            if (now - _lastDebounceTime >= _debounceDelay) {
                if (isLogicPressed) {
                    _latchedEvent = ButtonEvent::PRESS;
                    _internalState = InternalState::PRESSED;
                    Serial.println("Button Pressed");
                    _pressStartTime = now;
                } else {
                    _internalState = InternalState::IDLE;
                }
            }
            break;
        case InternalState::PRESSED:
            if (!isLogicPressed) {
                _latchedEvent = ButtonEvent::CLICK;
                _internalState = InternalState::IDLE;
                Serial.println("Button Clicked");
            }
            else if (now - _pressStartTime >= _longPressThreshold) {
                _latchedEvent = ButtonEvent::LONG_PRESS;
                _internalState = InternalState::LONG_HELD;
                Serial.println("Button Long Pressed");
            }
            break;
        case InternalState::LONG_HELD:
            if (!isLogicPressed) {
                _internalState = InternalState::IDLE;
                _latchedEvent = ButtonEvent::NONE; // 떼는 순간 발생하는 클릭 이벤트를 명시적으로 차단
            }
            break;
    }
}

ButtonEvent Button::pullEvent() {
    ButtonEvent evt = _latchedEvent;
    _latchedEvent = ButtonEvent::NONE;
    return evt;
}

bool Button::isPressed() const {
    return (_internalState == InternalState::PRESSED || 
        _internalState == InternalState::LONG_HELD);
}
