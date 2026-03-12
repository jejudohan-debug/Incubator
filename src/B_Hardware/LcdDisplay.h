#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "A_Core/CoreTypes.h"
#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"
#include "LcdSetupDisplay.h"
#include "LcdNormalDisplay.h"

class LcdDisplay    // : public ILcdDisplay
{
private:
    LiquidCrystal_I2C _lcd{0x27, 16, 2};
 
    LcdNormalDisplay _normalDisplay;
    LcdSetupDisplay _setupDisplay;

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();

public:
    LcdDisplay() : _normalDisplay(_lcd), _setupDisplay(_lcd) {}
    virtual ~LcdDisplay() {}

    virtual void init();
    virtual void update();
    void setupCustomChars();
    void blinkBacklight();

    // void switchToSetup();
    // void switchToNormal();
};