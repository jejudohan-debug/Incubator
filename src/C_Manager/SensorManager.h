#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwareFactory.h"

class SensorManager : public IManager {
private:
    DisplayState& _view = SystemContext::getView();
    EventQueue& _queue = SystemContext::getQueue();

    DHTSensor& _dhtSensor = HardwareFactory::getDHTSensor();

public:
    SensorManager() = default;
    SensorManager(const SensorManager&) = delete;
    SensorManager& operator=(const SensorManager&) = delete;

    virtual ~SensorManager() {}

    void init();
    void update();
};