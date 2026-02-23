#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "A_Core/CoreTypes.h"
#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"
#include "LcdSetupDisplay.h"
#include "LcdNormalDisplay.h"

class LcdDisplay : public ILcdDisplay
{
private:
    LiquidCrystal_I2C &_lcd = SystemContext::getLcd();
    DisplayState &_view = SystemContext::getView();

    LcdNormalDisplay _normalDisplay;
    LcdSetupDisplay _setupDisplay;
    ILcdDisplay *_currentDisplay;

public:
    LcdDisplay() {}
    virtual ~LcdDisplay() {}

    virtual void init() override;
    virtual void update() override;
    virtual void clear() override;

    void switchToSetup();
    void switchToNormal();
    void setupCustomChars();
};