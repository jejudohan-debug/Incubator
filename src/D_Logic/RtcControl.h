#pragma once
#include "A_Core/SystemContext.h"
#include "C_Manager/ConfigEEPROM.h"

const int WEEKLY_DROP = 25; // 주당 2.5도씩 감소

class RtcControl
{
private:
    ConfigEEPROM &_cfgEEPROM;

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    // ActionQueue &_queue = SystemContext::getInstance().getActionQueue();

    bool _isLimitReached = true;

public:
    RtcControl(ConfigEEPROM &cfgEEPROM) : _cfgEEPROM(cfgEEPROM) {}

    void handleAction(SystemAction action);

    void turnStrategy();
    void calculateBrooderTemp();
};