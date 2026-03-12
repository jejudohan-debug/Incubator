#include "LcdSetupDisplay.h"

const char title0[] PROGMEM = "Setup: Species. ";
const char title1[] PROGMEM = "Setup: Day.     ";
const char title2[] PROGMEM = "Setup: Hour.    ";
const char title3[] PROGMEM = "Setup: Minute.  ";
const char title4[] PROGMEM = "Setup: Temp.    ";
const char title5[] PROGMEM = "Setup: Humi.    ";
const char title6[] PROGMEM = "Setup: INTERVAL.";
const char title7[] PROGMEM = "Setup: DURATION.";
const char *const LcdSetupDisplay::setupTitle[] PROGMEM = {
    title0, title1, title2, title3, title4, title5, title6, title7};

const LcdSetupDisplay::RenderFunc LcdSetupDisplay::_renderTable[] PROGMEM = {
    &LcdSetupDisplay::renderSpecies,
    &LcdSetupDisplay::renderDay,
    &LcdSetupDisplay::renderHour,
    &LcdSetupDisplay::renderMinute,
    &LcdSetupDisplay::renderTemperature,
    &LcdSetupDisplay::renderHumidity,
    &LcdSetupDisplay::renderTurnInterval,
    &LcdSetupDisplay::renderTurnDuration};

void LcdSetupDisplay::printFormatValue(float value)
{
    char buf[6];
    uint16_t val = value * 10.0f;

    if (val > 999)
        val = 999;

    uint16_t integral = val / 10;
    uint8_t fractional = val % 10;

    buf[0] = (integral / 10) + '0';
    buf[1] = (integral % 10) + '0';
    buf[2] = '.';
    buf[3] = fractional + '0';
    buf[4] = '\0';

    _lcd.print(buf);
}

void LcdSetupDisplay::update()
{
    if (_view.getPageStep() < PageStep::SETUP_FIRST ||
        _view.getPageStep() >= PageStep::COUNT)
    {
        return;
    }

    uint8_t stepIdx = _view.getPageStep() - PageStep::SETUP_FIRST;

    // 확인 대기 상태이면 renderConfirm() 호출
    if (_operate.getWaiting())
    {
        renderConfirmTitle(stepIdx);
        renderSetupValue(stepIdx);
        return;
    }

    if (_view.updateFlags)
    {
        renderSetupTitle(stepIdx);
        renderSetupValue(stepIdx);
    }
}

void LcdSetupDisplay::renderSetupTitle(uint8_t index)
{
    _lcd.setCursor(0, 0);
    // Flash 메모리에서 문자열 포인터를 읽어옴
    const char *ptr = (const char *)pgm_read_ptr(&setupTitle[index]);
    // 읽어온 주소의 문자열을 출력 (Flash 전용 출력)
    _lcd.print((const __FlashStringHelper *)ptr);
}

// 이동 시 확인 화면 출력
void LcdSetupDisplay::renderConfirmTitle(uint8_t index)
{
    // 현재 설정값 표시
    _lcd.setCursor(0, 0);
    _lcd.print(F("Save? UP:Y DN:N "));
}

void LcdSetupDisplay::renderSetupValue(uint8_t index)
{
    _lcd.setCursor(0, 1);
    _lcd.print(F("Value:          "));
    _lcd.setCursor(7, 1); // "Value: " 가 7자이므로 그 바로 뒤부터 출력

    const int mapSize = sizeof(_renderTable) / sizeof(_renderTable[0]);
    if (index < mapSize)
    {
        RenderFunc func;
        memcpy_P(&func, &_renderTable[index], sizeof(RenderFunc));
        if (func != nullptr)
        {
            (this->*func)();
        }
    }
}

// 스텝별 출력 함수 구현
void LcdSetupDisplay::renderSpecies()
{
    const char *pgm_ptr = _species.getSpeciesName(_view.getSpecies());
    _lcd.print((const __FlashStringHelper *)pgm_ptr);
}

void LcdSetupDisplay::renderDay()
{
    _lcd.print(_view.getElapsedDay());
    _lcd.print(F(" day"));
}

void LcdSetupDisplay::renderHour()
{
    _lcd.print(_view.getElapsedHour());
    _lcd.print(F(" hour"));
}

void LcdSetupDisplay::renderMinute()
{
    _lcd.print(_view.getElapsedMinute());
    _lcd.print(F(" min"));
}

void LcdSetupDisplay::renderTemperature()
{
    //_lcd.print(formatPaddedFloat(_view.getTargetTemp(), 5, 1));
    printFormatValue(_view.getTargetTempFloat());
    _lcd.write(223);
    _lcd.print('C');
}

void LcdSetupDisplay::renderHumidity()
{
    //_lcd.print(formatPaddedFloat(_view.targetHumi, 5, 1));
    printFormatValue(_view.getTargetHumiFloat());
    _lcd.print('%');
}

void LcdSetupDisplay::renderTurnInterval()
{
    _lcd.print(_view.getTurnInterval());
    _lcd.print(F(" min"));
}

void LcdSetupDisplay::renderTurnDuration()
{
    _lcd.print(_view.getTurnDuration());
    _lcd.print(F(" sec"));
}
