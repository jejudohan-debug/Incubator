#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"

// 부화 기간을 21일(기본값)로 가정했을 때의 계산
const uint8_t BAR_BLOCK_COUNT = 6;
const uint8_t PIXELS_PER_BLOCK = 5;
const uint8_t BAR_START_COL = 9;
const uint8_t TOTAL_DAYS = 21;

class LcdDisplay;

class LcdNormalDisplay : public ILcdDisplay
{
private:
    LiquidCrystal_I2C &_lcd = SystemContext::getLcd();
    DisplayState &_view = SystemContext::getView();

    using RenderFunc = void (LcdNormalDisplay::*)();
    static const RenderFunc _renderTable[];

public:
    LcdNormalDisplay() {}
    virtual ~LcdNormalDisplay() {}

    virtual void init() override {}
    virtual void update() override;
    virtual void clear() override;

    void renderTime();
    void animateProgressBsr();
    void drawProgressBar(uint8_t filledPixels, uint8_t maxPixels);
    void renderEnv();
    void renderRelays();
    void displayConfig();
};
