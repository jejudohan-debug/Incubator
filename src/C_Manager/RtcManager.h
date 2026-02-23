#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwareFactory.h"

class RtcManager {
private:
    RTCDevice& _rtc = HardwareFactory::getRTCDevice();

public:
    RtcManager() = default;
    void init() {
        _rtc.init();
    }

    uint32_t getCurrentTime() {
        return _rtc.getUnixTime();
    }

    void setCurrentTime(uint32_t unixTime) {
        _rtc.setTime(unixTime);
    }

    bool saveConfig(const SystemConfig& config) {
        return _rtc.saveConfig(config);
    }
};