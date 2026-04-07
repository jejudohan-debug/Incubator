#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "A_Core/SystemContext.h"
#include "A_Core/Interfaces.h"

class LcdSetupDisplay // : public ILcdDisplay
{
private:
    using RenderFunc = void (LcdSetupDisplay::*)();
    static const RenderFunc _renderTable[];
    static const char *const setupTitle[];

    LiquidCrystal_I2C &_lcd;

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    SpeciesContext &_species = SystemContext::getInstance().getSpeciesContext();

public:
    LcdSetupDisplay(LiquidCrystal_I2C &lcd)
        : _lcd(lcd) {}

    virtual void init() {}
    virtual void update();

private:
    void renderConfirmTitle(uint8_t index);
    void renderSetupTitle(uint8_t index);
    void renderSetupValue(uint8_t index);

    void printFormatValue(uint16_t value);

    void renderSpecies();
    void renderDay();
    void renderHour();
    void renderMinute();
    void renderTemperature();
    void renderHumidity();
    void renderTurnInterval();
    void renderTurnDuration();
    void renderPID_Kp();
    void renderPID_Ki();
    void renderPID_Kd();
};