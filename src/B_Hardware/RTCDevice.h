
#pragma once

#include <Wire.h>
#include <RtcLib.h>
#include "A_Core/Interfaces.h"

class RTCDevice : public IDevice
{
private:
    RTC_DS3231 _rtc;

    unsigned long _lastReadTime = 0;

public:
    RTCDevice() {}

    void init() override;
    void update() override;

    uint32_t getUnixTime();
    void setTime(uint32_t timestamp);
};