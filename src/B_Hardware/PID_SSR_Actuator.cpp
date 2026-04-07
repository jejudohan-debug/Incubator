#include "PID_SSR_Actuator.h"

void PID_SSR_Actuator::init()
{
    pinMode(_pin, OUTPUT);
    stop(); // 초기 상태는 Off
}

void PID_SSR_Actuator::setOutput(int16_t val)
{
    if (val < 0)
        val = 0;
    else if (val > 1000)
        val = 1000;
    _output = val;
}

void PID_SSR_Actuator::stop()
{
    _output = 0;
    update();
}

void PID_SSR_Actuator::update()
{
    // update()는 loop()에서 최대한 자주 호출되어야 합니다.
    controlHeater((uint16_t)_output);
}

void PID_SSR_Actuator::controlHeater(uint16_t pidOutput) {
    static unsigned long windowStartTime = millis();
    static bool lastPinState = false; // 이전 출력 상태 저장

    if (millis() - windowStartTime >= SSR_CONTROL_INTERVAL) {
        windowStartTime = millis();
        //Serial.print(F("PID_SSR_Actuator::controlHeater: "));
        //Serial.println(pidOutput);
    }

    //unsigned long onTime = (SSR_CONTROL_INTERVAL * (unsigned long)pidOutput) / 255;
    _state = (millis() - windowStartTime < pidOutput);

    // 실제 핀에 나갈 논리값 결정
    bool currentPinState = _activeLow ? !_state : _state;

    // 상태가 변했을 때만 실행
    if (currentPinState != lastPinState) {
        digitalWrite(_pin, currentPinState ? HIGH : LOW);
        lastPinState = currentPinState;
    }
}