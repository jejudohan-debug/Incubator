#pragma once
#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwarePins.h"
#include "B_Hardware/Button.h"

#define BUTTON_CNT 3

class EventBtnManager : public IManager
{
private:
    Button::Group<3> &_btns;

    DisplayState &_view = SystemContext::getInstance().getView();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

    void handleBtnSelect(const ButtonEvent evt);
    void handleBtnUp(const ButtonEvent evt);
    void handleBtnDown(const ButtonEvent evt);

public:
    EventBtnManager(Button::Group<3> &btns)
        : _btns(btns) {}
    void init() override;
    void update() override;
    void onNotify(EventFlag::Type flag, const uint8_t value);
};