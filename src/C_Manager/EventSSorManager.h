#pragma once
#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwarePins.h"
#include "B_Hardware/SHTSensor.h"

class EventSSorManager : public IManager
{
private:
    // SHTSensor _shtSensor{Pins::SHT};

    SHTSensor &_sht;

    DisplayState &_view = SystemContext::getInstance().getView();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

    // void handleShtTemp(const float value);
    // void handleShtHumi(const float value);

public:
    EventSSorManager(SHTSensor &sht);
    void init();
    void update();

    // void handleEvent(EventFlag::Type flag, const float value);
    void onNotify(EventFlag::Type flag, const uint16_t value);
};