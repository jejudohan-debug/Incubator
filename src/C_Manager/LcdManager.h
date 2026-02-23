#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwareFactory.h"

class LcdManager
{
private:
    LcdDisplay &_display = HardwareFactory::getLcdDisplay();
    // DisplayState& _view = SystemContext().getView();

public:
    LcdManager() = default;

    LcdManager(const LcdManager &) = delete;
    LcdManager &operator=(const LcdManager &) = delete;

    virtual ~LcdManager() {}

    void init()
    {
        _display.init();
    }

    void update()
    {
        _display.update();
    };
};
