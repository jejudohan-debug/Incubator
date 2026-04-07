#pragma once
#include <Arduino.h>

// Nano용
namespace Pins
{
    // --- 센서 ---
    //constexpr uint8_t DHT = 4; SHT30으로 교체 0x44 주소 사용

    // --- 출력 장치 ---
    constexpr uint8_t TURN = 3;
    constexpr uint8_t HEATER = 6; // PWM 제어 가능
    constexpr uint8_t FAN = 5;
    constexpr uint8_t HUMIDIFIER = 12;

    // --- 입력 장치 (Buttons) ---
    constexpr uint8_t BTN_SELECT = 7;
    constexpr uint8_t BTN_UP = 8;
    constexpr uint8_t BTN_DOWN = 9;
    // constexpr uint8_t BTN_ENTER  = 9;
    constexpr uint8_t LIMIT_SW = 2;


    // RTC (3-Wire 방식) -> DS3231로 교체 0x68 주소 사용
    //constexpr uint8_t RTC_RST = 10;
    //constexpr uint8_t RTC_DAT = 11;
    //constexpr uint8_t RTC_CLK = 12;

    // LCD -> 0x27 주소 사용
    // 나노에서 A4, A5는 하드웨어 I2C 
};