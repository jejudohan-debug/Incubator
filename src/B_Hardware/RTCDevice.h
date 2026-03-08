
#pragma once

#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "A_Core/Interfaces.h"

class RTCDevice : public IDevice
{
private:
    ThreeWire _wire;
    RtcDS1302<ThreeWire> _rtc;
    // DeviceIdx _deviceIdx;
    // uint32_t _lastUnixTime = 0;
    unsigned long _lastReadTime = 0;

public:
    RTCDevice(uint8_t dat, uint8_t clk, uint8_t rst)
        : _wire(dat, clk, rst), _rtc(_wire) {}

    void init() override;
    void update() override;

    uint32_t getUnixTime();
    void setTime(uint32_t timestamp);
    void saveConfig(const SystemConfig &config);
    void loadConfig(SystemConfig &config);
};