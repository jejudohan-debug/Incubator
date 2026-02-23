#pragma once

#include <Arduino.h>
#include "C_Manager/ManagerFactory.h"
#include "InputControl.h"
#include "SensorControl.h"
#include "ControlFactory.h"
#include "TurningControl.h"

class IncubatorLogic
{
private:
    LcdManager* _lcdMgr = nullptr;
    InputControl* _inputControl = nullptr;
    SensorControl* _sensorCtl = nullptr;
    TurningControl* _turningCtl = nullptr;

    EventQueue &_queue = SystemContext::getQueue();

public:
    IncubatorLogic() = default;

    void init();
    void update();
};