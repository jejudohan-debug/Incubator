#pragma once

#include "HardwarePins.h"
#include "LcdDisplay.h"
#include "Button.h"
#include "DHTSensor.h"
#include "RTCDevice.h"
#include "RelayActuator.h"

class HardwareFactory
{
public:
    HardwareFactory() = default;

    // 복사 금지 (메모리 안정성 확보)
    HardwareFactory(const HardwareFactory &) = delete;
    HardwareFactory &operator=(const HardwareFactory &) = delete;

    static LcdDisplay &getLcdDisplay();
    static Button *getButtons();
    static DHTSensor &getDHTSensor();
    static RTCDevice &getRTCDevice();
};
