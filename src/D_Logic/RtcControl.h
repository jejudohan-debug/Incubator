#pragma once
#include "A_Core/SystemContext.h"
// #include "B_Hardware/HardwareFactory.h"

class RtcControl
{
private:
    DisplayState &_view = SystemContext::getInstance().getView();
    ActionQueue &_queue = SystemContext::getInstance().getActionQueue();

public:
    RtcControl() {}

    void handleAction(SystemAction action);

    void turnStrategy();
};