
#pragma once

#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "A_Core/Interfaces.h"

class RTCDevice {
private:
    ThreeWire _wire;
    RtcDS1302<ThreeWire> _rtc;

public:
    RTCDevice(uint8_t dat, uint8_t clk, uint8_t rst) 
        : _wire(dat, clk, rst), _rtc(_wire) {}

    void init() {
        _rtc.Begin();
        
        // 시간 기록이 멈춰있다면(새 칩인 경우) 작동 시작
        if (!_rtc.GetIsRunning()) {
            _rtc.SetIsRunning(true);
        }

        /* 컴파일 시간으로 초기화 (필요시)
        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        if (_rtc.GetDateTime() < compiled) {
            _rtc.SetDateTime(compiled);
        }*/
    }

    void update() {
        
    }

    uint32_t getUnixTime() {
        if (!_rtc.GetIsRunning()) return 0;
        return _rtc.GetDateTime().Unix32Time();
    }

    void setTime(uint32_t timestamp) {
        _rtc.SetDateTime(RtcDateTime(timestamp));
    }

    bool saveConfig(const SystemConfig& config) {
        // 구조체를 바이트 배열로 캐스팅하여 저장
        const uint8_t* data = reinterpret_cast<const uint8_t*>(&config);
        
        if (sizeof(SystemConfig) > 31) return false; // DS1302 메모리 한계

        for (uint8_t i = 0; i < sizeof(SystemConfig); i++) {
            _rtc.SetMemory(i, data[i]);
        }
        return true;
    }
    
    bool loadConfig(SystemConfig& config) {
        uint8_t* data = reinterpret_cast<uint8_t*>(&config);
        
        for (uint8_t i = 0; i < sizeof(SystemConfig); i++) {
            data[i] = _rtc.GetMemory(i);
        }
        return true;
    }
};