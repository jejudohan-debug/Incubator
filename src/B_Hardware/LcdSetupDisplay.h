#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "A_Core/SystemContext.h"
#include "A_Core/Interfaces.h"

class LcdSetupDisplay : public ILcdDisplay
{
private:
    LiquidCrystal_I2C &_lcd = SystemContext::getLcd();
    DisplayState &_view = SystemContext::getView();

    using RenderFunc = void (LcdSetupDisplay::*)();
    static const RenderFunc _renderTable[];
    static const char *setupTitle[];

public:
    LcdSetupDisplay() {}
    virtual ~LcdSetupDisplay() {}

    virtual void init() override {}
    virtual void update() override;
    virtual void clear() override;

    void renderSpecies();
    void renderDay();
    void renderHour();
    void renderMinute();
    void renderTemperature();
    void renderHumidity();
    void renderTurnInterval();

    // 확인 대기 상태 렌더링
    void renderConfirm();
};