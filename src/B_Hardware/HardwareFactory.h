#pragma once

#include "HardwarePins.h"
#include "LcdDisplay.h"
#include "Button.h"
#include "DHTSensor.h"
#include "RTCDevice.h"
#include "RelayActuator.h"
// #include "EEPROMDevice.h"
//  #include "PID_SSR_Actuator.h"

class HardwareFactory
{
public:
    HardwareFactory() = default;

    // 복사 금지 (메모리 안정성 확보)
    HardwareFactory(const HardwareFactory &) = delete;
    HardwareFactory &operator=(const HardwareFactory &) = delete;

    static HardwareFactory &getInstance();
    LcdDisplay &getDisplay();
    DHTSensor &getDHT();
    RTCDevice &getRTC();
    // EEPROMDevice &getEEPROM();
    //  PID_SSR_Actuator &getSSR();
    Button::Group<BUTTON_CNT> &getButtons();
    RelayActuator::Group<RELAY_CNT> &GetRelays();
};
