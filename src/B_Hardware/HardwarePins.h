#pragma once
#include <Arduino.h>

// Nano용
namespace Pins
{
    // --- 센서 ---
    constexpr uint8_t DHT = 2;

    // --- 출력 장치 ---
    constexpr uint8_t TURN = 4;
    constexpr uint8_t HEATER = 3; // PWM 제어 가능
    constexpr uint8_t FAN = 5;

    // --- 입력 장치 (Buttons) ---
    constexpr uint8_t BTN_SELECT = 7;
    constexpr uint8_t BTN_UP = 8;
    constexpr uint8_t BTN_DOWN = 9;
    // constexpr uint8_t BTN_ENTER  = 9;
    constexpr uint8_t LIMIT_SW = 6;

    // --- RTC (3-Wire 방식) ---
    constexpr uint8_t RTC_RST = 10;
    constexpr uint8_t RTC_DAT = 11;
    constexpr uint8_t RTC_CLK = 12;

    // --- I2C (LCD) ---
    // 나노에서 A4, A5는 하드웨어 I2C 핀으로 고정되어 있습니다.
};