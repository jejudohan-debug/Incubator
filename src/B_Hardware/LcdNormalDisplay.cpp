#include "LcdNormalDisplay.h"

// 부화 기간을 21일(기본값)로 가정했을 때의 계산
const uint8_t BAR_BLOCK_COUNT = 6;
const uint8_t PIXELS_PER_BLOCK = 5;
const uint8_t BAR_START_COL = 9;
//const uint8_t TOTAL_DAYS = 21;

void LcdNormalDisplay::printFormatInt(const __FlashStringHelper *label, int value)
{
    if (label)
        _lcd.print(label);
    if (value < 10)
        _lcd.write('0');
    _lcd.print(value);
}

void LcdNormalDisplay::printFormatUint16(const __FlashStringHelper *label, uint16_t value)
{
    if (label)
        _lcd.print(label);

    char buf[6];
    _view.uint16ToString(buf, value);
    _lcd.print(buf);
}

/*void LcdNormalDisplay::printFormatValue(uint16_t value)
{
    char buf[6];

    if (value > 999)
        value = 999;

    uint16_t integral = value / 10;
    uint8_t fractional = value % 10;

    buf[0] = (integral / 10) + '0';
    buf[1] = (integral % 10) + '0';
    buf[2] = '.';
    buf[3] = fractional + '0';
    buf[4] = '\0';

    _lcd.print(buf);
}*/

void LcdNormalDisplay::update()
{
    if (_view.getPageStep() == PageStep::ENV)
    {
        if (_view.updateFlags)
        {
            renderEnv();
            renderRelay();
            renderTime();
        }
        animateProgressBsr();
    }
    else
    {
        if (_view.updateFlags)
        {
            renderConfig();
        }
    }
}

void LcdNormalDisplay::renderConfig()
{
    SpeciesProfile profile = _species.getProfile(_view.getSpecies());
    _lcd.setCursor(0, 0);
    printFormatInt(F("Day:"), profile.totalDays);
    printFormatInt(F(",Hat:"), profile.hatchStartDay);

    renderRelay();

    _lcd.setCursor(0, 1);
     printFormatUint16(F("Hatch Humi:"), (int)profile.hatchHumi);
    _lcd.write('%');
}

void LcdNormalDisplay::renderEnv()
{
    _lcd.setCursor(0, 0);
    printFormatUint16(nullptr, _view.getCurrentTempFixed());
    _lcd.write(223);
    printFormatUint16(F("C, "), _view.getCurrentHumiFixed());
    _lcd.write(37);
}

void LcdNormalDisplay::renderRelay()
{  
    _lcd.setCursor(13, 0);
    _lcd.write(_operate.getRelayHeat() ? 'H' : ' ');
    _lcd.write(_operate.getRelayFan() ? 'F' : ' ');
    _lcd.write(_operate.getRelayTurn() ? 'T' : ' ');
}

void LcdNormalDisplay::renderTime()
{
    _lcd.setCursor(0, 1);
    printFormatInt(nullptr, _view.getElapsedDay());
    printFormatInt(F("d "), _view.getElapsedHour());
    printFormatInt(F(":"), _view.getElapsedMinute());
    //printFormatInt(F(":"), _view.getElapsedUnixTime() % 60);
}

void LcdNormalDisplay::animateProgressBsr()
{
    // 상태바 에니매이션
    static unsigned long _lastAnimTime = 0;
    static uint8_t _animStep = 0;
    static uint8_t _waitCount = 0;
    static uint8_t _targetPixels = 0;
    const unsigned long ANIM_INTERVAL = 500; // 0.5초마다 애니메이션 업데이트

    auto& speciesCtx = SystemContext::getInstance().getSpeciesContext();
    Species currentSpecies = _view.getSpecies();
    uint8_t totalDays = speciesCtx.getTotalDays(currentSpecies);

    if ((millis() - _lastAnimTime) < ANIM_INTERVAL)
        return;

    _lastAnimTime = millis();
    _targetPixels = (_view.getElapsedDay() * BAR_BLOCK_COUNT * PIXELS_PER_BLOCK) / totalDays;
    if (_targetPixels <= 0)
        _targetPixels = 1;

    _lcd.setCursor(BAR_START_COL, 1);
    uint8_t maxPixels = BAR_BLOCK_COUNT * PIXELS_PER_BLOCK - 1;
    if (_animStep >= _targetPixels)
    {
        _waitCount++;
        uint8_t blinkStep = (_waitCount % 2 == 0) ? _targetPixels : (_targetPixels - 1);
        drawProgressBar(blinkStep, maxPixels);
        if (_waitCount >= 10)
        { // 10번 애니메이션이 끝나면 대기 후 초기화
            _animStep = 0;
            _waitCount = 0;
        }
    }
    else
    {
        _animStep++;
        drawProgressBar(_animStep, maxPixels);
    }
}

void LcdNormalDisplay::drawProgressBar(uint8_t filledPixels, uint8_t maxPixels)
{
    _lcd.setCursor(BAR_START_COL, 1);
    _lcd.write(6); // 벽 문자

    // 2. 바 렌더링 (15번 칸부터 9번 칸까지 역순)
    for (int8_t i = 0; i < BAR_BLOCK_COUNT; i++)
    {
        uint8_t blockIndex = (BAR_BLOCK_COUNT - 1) - i;
        uint8_t startPixelOfBlock = blockIndex * PIXELS_PER_BLOCK;

        if (filledPixels >= startPixelOfBlock + PIXELS_PER_BLOCK)
        {
            _lcd.write(5); // 꽉 찬 칸
        }
        else if (filledPixels > startPixelOfBlock)
        {
            _lcd.write(filledPixels - startPixelOfBlock); // 1~4픽셀
        }
        else
        {
            _lcd.write(0); // 빈 레일
        }
    }
}

/*void LcdNormalDisplay::renderProgressBar() {
    uint8_t filledPixels =
        (_view.d * BAR_BLOCK_COUNT * PIXELS_PER_BLOCK) / TOTAL_DAYS;
    if (filledPixels <= 0) filledPixels = 1;

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
}*/