#include "LcdDisplay.h"

// void LcdDisplay::clear() { _lcd.clear(); }

void LcdDisplay::init()
{
    Serial.println(F("Initializing LCD..."));
    Serial.flush();
    _lcd.init();
    _lcd.backlight();
    setupCustomChars();

    _view.updateFlags = UpdateFlag::ALL;
}

void LcdDisplay::setupCustomChars()
{
    // 1. 데이터 정의 (Flash 메모리에 저장하여 RAM 절약)
    const uint8_t barChars[7][8] = {
        {0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F}, // 0: 빈 레일 [=]
        {0x1F, 0x1F, 0x01, 0x01, 0x01, 0x01, 0x1F, 0x1F}, // 1: 1픽셀
        {0x1F, 0x1F, 0x03, 0x03, 0x03, 0x03, 0x1F, 0x1F}, // 2: 2픽셀
        {0x1F, 0x1F, 0x07, 0x07, 0x07, 0x07, 0x1F, 0x1F}, // 3: 3픽셀
        {0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x0F, 0x1F, 0x1F}, // 4: 4픽셀
        {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // 5: 꽉 참 [■]
        {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}  // 6: 벽 [|
    };

    for (int i = 0; i < 7; i++)
    {
        _lcd.createChar(i, (uint8_t *)barChars[i]);
    }
}

void LcdDisplay::update()
{
    if (_view.getAlert())
    {
        blinkBacklight();
        return;
    }

    //if (!_view.updateFlags)
    //    return;

    if (UpdateFlag::hasFlag(_view.updateFlags, UpdateFlag::STEP))
    {
        _lcd.clear();
    }

    if ((_view.getPageStep() == PageStep::ENV) || (_view.getPageStep() == PageStep::CONFIG))
    {
        _normalDisplay.update();
    }
    else
    {
        _setupDisplay.update();
    }
}

void LcdDisplay::blinkBacklight()
{
    static uint32_t lastBlinkTime = 0;
    static bool lightOn = true;

    if (millis() - lastBlinkTime >= 500)
    {
        lastBlinkTime = millis();
        lightOn = !lightOn;

        if (lightOn)
        {
            _lcd.backlight();
        }
        else
        {
            _lcd.noBacklight();
        }
    }
}