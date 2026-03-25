#pragma once
#include "A_Core/SystemContext.h"
// #include "B_Hardware/HardwareFactory.h"

class SSorControl
{
private:
    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    //ActionQueue &_queue = SystemContext::getInstance().getActionQueue();

    unsigned long _heatingOnTime = 0;
    unsigned long _heatingOffTime = 0;
    uint16_t _startTemp;

public:
    SSorControl() {}

    void handleAction(SystemAction action);

    //void tempStrategy();
    void humiStrategy();
    void checkHeatHealth();
};