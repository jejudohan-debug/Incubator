#pragma once
#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwarePins.h"
#include "B_Hardware/Button.h"

class EventBtnManager : public IManager
{
private:
    Button::Group<BUTTON_CNT> &_btns;

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

    void handleBtnSelect(const ButtonEvent evt);
    void handleBtnUp(const ButtonEvent evt);
    void handleBtnDown(const ButtonEvent evt);
    void handleLimitSw(const ButtonEvent evt);

public:
    EventBtnManager(Button::Group<BUTTON_CNT> &btns)
        : _btns(btns) {}
    void init() override;
    void update() override;
    void onNotify(EventFlag::Type flag, const uint8_t value);
};