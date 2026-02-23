#pragma once

#include <LiquidCrystal_I2C.h>
#include "CoreTypes.h"

// @brief 부화기 하드웨어 장치를 위한 최상위 인터페이스
// 모든 드라이버 클래스(Heater, Sensor 등)는 이 클래스를 상속받아야 합니다.
class IDevice
{
public:
    virtual ~IDevice() {}
    virtual void init() = 0;
    virtual void update() = 0;
};

// @brief 모든 출력 장치(Actuator)를 위한 인터페이스
// 히터, 팬, 부저, 모터 등 '동작'을 수행하는 장치들이 상속받습니다.
class IActuator
{
public:
    virtual ~IActuator() {}

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() const = 0;
    // @brief (선택) 장치의 출력을 세밀하게 조절합니다.
    // @param power 0~255 사이의 출력 값 (PWM 제어 등)
    // 기본적으로는 아무 동작도 하지 않도록 정의합니다.
    virtual void setPower(uint8_t power)
    {
        (void)power; // 미사용 변수 경고 방지
    }
};

class ILcdDisplay
{
public:
    virtual ~ILcdDisplay() {}
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void clear() = 0;

protected:
    const char *formatPaddedFloat(float val, uint8_t width, uint8_t prec)
    {
        static char buffer[20];
        dtostrf(val, width, prec, buffer);
        return buffer;
    }

    const char *formatDigits(int digits, bool zeroPadding)
    {
        static char buf[3]; // 2자리 숫자 + 널 문자를 위한 공간

        // 십의 자리
        int tens = digits / 10;
        if (tens == 0)
        {
            buf[0] = zeroPadding ? '0' : ' '; // 0 또는 공백 패딩
        }
        else
        {
            buf[0] = (tens % 10) + '0'; // 십의 자리 숫자를 문자로 변환
        }

        // 일의 자리
        buf[1] = (digits % 10) + '0';

        // 문자열 끝 지정
        buf[2] = '\0';

        return buf;
    }
};

class IManager
{
public:
    virtual ~IManager() {}
    virtual void init() = 0;
    virtual void update() = 0;
};

class IControl
{
public:
    virtual ~IControl() {}
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void handleAction(SystemAction action) = 0;
};