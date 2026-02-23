//#include "LcdDevice.h"
#include "LcdNormalDisplay.h"
#include "LcdDisplay.h"

const LcdNormalDisplay::RenderFunc LcdNormalDisplay::_renderTable[] = {
    &LcdNormalDisplay::renderEnv,
    &LcdNormalDisplay::renderRelays,
    &LcdNormalDisplay::displayConfig
};

void LcdNormalDisplay::clear() { _lcd.clear(); }
void LcdNormalDisplay::update()
{
    if (_view.isUpdated) {
        _lcd.clear();
        _lcd.setCursor(0,0);
        uint8_t pageIdx = static_cast<uint8_t>(_view.normal.page);
        (this->*_renderTable[pageIdx])();

        _lcd.setCursor(0,1);
        renderTime();

        _view.isUpdated = false;
    }
    animateProgressBsr();
}

void LcdNormalDisplay::renderTime() {
    _lcd.print(formatDigits(_view.d, true));
    _lcd.print(F("d "));
    _lcd.print(formatDigits(_view.h, true));
    _lcd.print(':');
    _lcd.print(formatDigits(_view.m, true));
}

void LcdNormalDisplay::animateProgressBsr() {
    // 상태바 에니매이션
    static unsigned long _lastAnimTime = 0;
    static uint8_t _animStep = 0;
    static uint8_t _waitCount = 0;
    static uint8_t _targetPixels = 0;
    const unsigned long ANIM_INTERVAL = 500; // 0.5초마다 애니메이션 업데이트

    if ((millis() - _lastAnimTime) < ANIM_INTERVAL) return;

    _lastAnimTime = millis();
    _targetPixels = (_view.d * BAR_BLOCK_COUNT * PIXELS_PER_BLOCK) / TOTAL_DAYS;
    if (_targetPixels <= 0) _targetPixels = 1;

    _lcd.setCursor(BAR_START_COL, 1);
    uint8_t maxPixels = BAR_BLOCK_COUNT * PIXELS_PER_BLOCK - 1;
    if (_animStep >= _targetPixels) {
        _waitCount++;
        uint8_t blinkStep = (_waitCount % 2 == 0) ? _targetPixels : (_targetPixels - 1);
        drawProgressBar(blinkStep, maxPixels);
        if (_waitCount >= 10) { // 10번 애니메이션이 끝나면 대기 후 초기화
            _animStep = 0;
            _waitCount = 0;
        }
    } else {
        _animStep++;
        drawProgressBar(_animStep, maxPixels);
    }
}

void LcdNormalDisplay::drawProgressBar(uint8_t filledPixels, uint8_t maxPixels) {
    _lcd.write(6); // 벽 문자

    // 2. 바 렌더링 (15번 칸부터 9번 칸까지 역순)
    for (int8_t i = 0; i < BAR_BLOCK_COUNT; i++) {
        uint8_t blockIndex = (BAR_BLOCK_COUNT - 1) - i;  
        uint8_t startPixelOfBlock = blockIndex * PIXELS_PER_BLOCK;

        if (filledPixels >= startPixelOfBlock + PIXELS_PER_BLOCK) {
            _lcd.write(5); // 꽉 찬 칸
        } 
        else if (filledPixels > startPixelOfBlock) {
            _lcd.write(filledPixels - startPixelOfBlock); // 1~4픽셀
        } 
        else {
            _lcd.write(0); // 빈 레일
        }
    }
}

void LcdNormalDisplay::renderEnv() {
    _lcd.print(F("T:"));
    _lcd.print(formatPaddedFloat(_view.normal.currentTemp, 4, 1));
    _lcd.write(223);
    _lcd.print(F("C H:"));
    _lcd.print(formatPaddedFloat(_view.normal.currentHumi, 4, 1));
    _lcd.write(37);
}

void LcdNormalDisplay::renderRelays() {
    _lcd.print(F("RY H:"));
    _lcd.print(_view.normal.relayBits & 0x01 ? "O" : "X");
    _lcd.print(F(" F:"));
    _lcd.print(_view.normal.relayBits & 0x02 ? "O" : "X");
    _lcd.print(F(" M:"));
    _lcd.print(_view.normal.relayBits & 0x04 ? "O" : "X");
}

void LcdNormalDisplay::displayConfig() {
    _lcd.print(F("Species:"));
    _lcd.print(SpeciesContext::getSpeciesName(_view.species));
}