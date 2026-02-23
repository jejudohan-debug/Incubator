#include "LcdSetupDisplay.h"

const char *LcdSetupDisplay::setupTitle[] = {
    "Setup: Species",
    "Setup: Day",
    "Setup: Hour",
    "Setup: Minute",
    "Setup: Temperata",
    "Setup: Humidity",
    "Setup: Turn Intv"};

const LcdSetupDisplay::RenderFunc LcdSetupDisplay::_renderTable[] = {
    &LcdSetupDisplay::renderSpecies,
    &LcdSetupDisplay::renderDay,
    &LcdSetupDisplay::renderHour,
    &LcdSetupDisplay::renderMinute,
    &LcdSetupDisplay::renderTemperature,
    &LcdSetupDisplay::renderHumidity,
    &LcdSetupDisplay::renderTurnInterval};

void LcdSetupDisplay::clear() { _lcd.clear(); }

void LcdSetupDisplay::update() {
    // 확인 대기 상태이면 renderConfirm() 호출
    if (_view.setup.isWaitingConfirm)
    {
        renderConfirm();
        return;
    }

    if (!_view.isUpdated) return;

    _lcd.clear();

    // 고정 텍스트는 Step이 바뀔 때 한 번만 출력
    uint8_t stepIdx = static_cast<uint8_t>(_view.setup.step);
    _lcd.setCursor(0, 0);
    _lcd.print(setupTitle[stepIdx]);

    _lcd.setCursor(0, 1);
    _lcd.print(F("Value: ")); // F() 매크로로 RAM 절약

    // 3. 실제 값(Value)만 업데이트 (해당 위치로 커서 이동 후 호출)
    _lcd.setCursor(7, 1); // "Value: " 가 7자이므로 그 바로 뒤부터 출력
    if (_renderTable[stepIdx])
    {
        (this->*_renderTable[stepIdx])();
    }
    _view.isUpdated = false;
}

// 이동 시 확인 화면 출력
void LcdSetupDisplay::renderConfirm() {
    if (!_view.isUpdated) return;

    _lcd.clear();

    // 현재 설정값 표시
    _lcd.setCursor(0, 0);
    _lcd.print(F("Save? UP:Y DN:N "));

    // 값 표시
    _lcd.setCursor(0, 1);
    _lcd.print(F("Value: "));
    uint8_t stepIdx = static_cast<uint8_t>(_view.setup.step);
    _lcd.setCursor(7, 1);
    if (_renderTable[stepIdx])
    {
        (this->*_renderTable[stepIdx])();
    }

    _view.isUpdated = false;
}

// 스텝별 출력 함수 구현
void LcdSetupDisplay::renderSpecies()
{
    //Serial.println(static_cast<uint8_t>(_view.species));
    _lcd.print(SpeciesContext::getSpeciesName(_view.species));
}

void LcdSetupDisplay::renderDay()
{
    _lcd.print(_view.d);
    _lcd.print(F(" day"));
}

void LcdSetupDisplay::renderHour()
{
    _lcd.print(_view.h);
    _lcd.print(F(" hour"));
}

void LcdSetupDisplay::renderMinute()
{
    _lcd.print(_view.m);
    _lcd.print(F(" min"));
}

void LcdSetupDisplay::renderTemperature()
{
    _lcd.print(formatPaddedFloat(_view.setup.targetTemp, 5, 1));
    _lcd.write(223);
    _lcd.print('C');
}

void LcdSetupDisplay::renderHumidity()
{
    _lcd.print(formatPaddedFloat(_view.setup.targetHumi, 5, 1));
    _lcd.print('%');
}

void LcdSetupDisplay::renderTurnInterval()
{
    _lcd.print(_view.setup.turnInterval);
    _lcd.print(F(" min"));
}