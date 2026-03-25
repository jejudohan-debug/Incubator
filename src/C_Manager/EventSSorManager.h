#pragma once
#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwarePins.h"
#include "B_Hardware/DHTSensor.h"

class EventSSorManager : public IManager
{
private:
    // DHTSensor _dhtSensor{Pins::DHT};
    DHTSensor &_dht;

    DisplayState &_view = SystemContext::getInstance().getView();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

    //void handleDhtTemp(const float value);
    //void handleDhtHumi(const float value);

public:
    EventSSorManager(DHTSensor &dht);
    void init();
    void update();

    // void handleEvent(EventFlag::Type flag, const float value);
    void onNotify(EventFlag::Type flag, const float value);
};