#include "PID_SSR_Actuator.h"

void PID_SSR_Actuator::init()
{
    pinMode(_pin, OUTPUT);
    stop(); // 초기 상태는 Off
}

// PID 연산 결과(0~255)를 업데이트하는 함수
void PID_SSR_Actuator::setOutput(double output)
{
    _output = constrain(output, 0, 255);
}

// 실제 SSR 제어: PWM 방식을 사용하거나 Time-Proportioning 방식을 사용
void PID_SSR_Actuator::update()
{
    // SSR은 반응이 빠르므로 analogWrite(PWM)를 사용할 수 있습니다.
    // 만약 AC SSR의 특성에 따라 PWM이 불안정하다면 Time-Proportioning 로직으로 변경 가능합니다.
    int rawValue = (int)_output;
    if (_activeLow)
    {
        analogWrite(_pin, 255 - rawValue);
    }
    else
    {
        analogWrite(_pin, rawValue);
    }
}

void PID_SSR_Actuator::stop()
{
    _output = 0;
    update();
}

int16_t PID_SSR_Actuator::computeIntegerPID(uint16_t target, uint16_t current)
{
    int32_t error = (int32_t)target - (int32_t)current;

    int32_t pTerm = Kp_int * error;

    static int16_t lastOutput = 0;
    if (lastOutput < 255 || error < 0)
    {
        integral += error;
    }
    integral = constrain(integral, -2000, 2000);
    int32_t iTerm = Ki_int * integral;

    int32_t dTerm = Kd_int * (error - lastError);
    int32_t total = (pTerm + iTerm + dTerm + 50) / 100;

    lastOutput = (int16_t)constrain(total, 0, 255);
    lastError = error;

    return lastOutput;
}

// PID 결과값이 0~255로 나온다고 가정할 때
void PID_SSR_Actuator::controlHeater(int pidOutput) {
    static unsigned long windowStartTime = millis();
    const unsigned long windowSize = 1000; // 1초(1000ms) 주기

    if (millis() - windowStartTime > windowSize) {
        windowStartTime += windowSize;
    }

    // 255를 1000ms로 매핑하여 켜지는 시간(onTime) 계산
    unsigned long onTime = map(pidOutput, 0, 255, 0, windowSize);

    if (onTime > (millis() - windowStartTime)) {
        digitalWrite(_pin, HIGH);
    } else {
        digitalWrite(_pin, LOW);
    }
}