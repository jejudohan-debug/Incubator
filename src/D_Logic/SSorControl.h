#pragma once
#include "A_Core/SystemContext.h"
// #include "B_Hardware/HardwareFactory.h"

class SSorControl
{
private:
    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    // ActionQueue &_queue = SystemContext::getInstance().getActionQueue();

    unsigned long _heatingOnTime = 0;
    //unsigned long _heatingOffTime = 0;
    uint16_t _startTemp;
    // unsigned long _humidifierStartTime = 0;
    // unsigned long _exhaustStartTime = 0;

        // PID 계산
    int32_t _integral = 0;
    int32_t _lastError = 0;
    int32_t _lastTemp = 0;
    bool _isRising = false;

public:
    SSorControl() {}

    void handleAction(SystemAction action);

    // void tempStrategy();
    void updateHeating();
    uint16_t computeIntegerPID();
    void humiStrategy();
    void exhaustStrategy(uint16_t currentTemp, uint16_t currentHumi);
    void checkHeatHealth();
};