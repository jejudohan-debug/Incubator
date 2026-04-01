#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"

class LcdNormalDisplay // : public ILcdDisplay
{
private:
    using RenderFunc = void (LcdNormalDisplay::*)();
    static const RenderFunc _renderTable[];

    LiquidCrystal_I2C &_lcd;

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    SpeciesContext &_species = SystemContext::getInstance().getSpeciesContext();

public:
    LcdNormalDisplay(LiquidCrystal_I2C &lcd)
        : _lcd(lcd) {}

    virtual void init() {}
    virtual void update();

private:
    void printFormatInt(const __FlashStringHelper *label, int value);
    void printFormatUint16(const __FlashStringHelper *label, uint16_t value);

    void renderEnv();
    void renderRelay();
    void renderConfig();
    void renderTime();
    // void renderProgressBar();
    void animateProgressBsr();
    void drawProgressBar(uint8_t filledPixels, uint8_t maxPixels);
    // void renderAutoTune();
};
