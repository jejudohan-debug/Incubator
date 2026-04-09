#include "SpeciesContext.h"

const char name_0[] PROGMEM = "Chicken";
const char name_1[] PROGMEM = "Duck";
const char name_2[] PROGMEM = "Goose";
const char name_3[] PROGMEM = "Quail";
const char name_4[] PROGMEM = "Brooder";
const char name_unknown[] PROGMEM = "Unknown";

const SpeciesProfile SpeciesContext::speciesTable[] PROGMEM = {
    // 부화기간, 파각시작일, 온도, 습도, 파각기습도,전란주기
    {name_0, 21, 18, 375, 450, 700, 180},
    {name_1, 28, 25, 372, 550, 800, 120},
    {name_2, 30, 27, 370, 550, 800, 120},
    {name_3, 17, 14, 377, 500, 750, 60},
    {name_4, 30, 0, 350, 500, 500, 0}};

/*SpeciesContext &SpeciesContext::getInstance()
{
    static SpeciesContext instance;
    return instance;
}*/

// 특정 종의 이름을 문자열로 가져옵니다.
const char *SpeciesContext::getSpeciesName(Species species)
{
    return getSpeciesName(static_cast<uint8_t>(species));
}
const char *SpeciesContext::getSpeciesName(uint8_t index)
{
    if (index >= static_cast<uint8_t>(Species::COUNT))
    {
        return name_unknown; // Flash 주소 반환
    }
    return (const char *)pgm_read_ptr(&speciesTable[index].name);
}
// Serial.println((__FlashStringHelper*)namePtr);
// lcd.print((__FlashStringHelper*)namePtr);

uint8_t SpeciesContext::getTotalDays(uint8_t index)
{
    if (index >= static_cast<uint8_t>(Species::COUNT))
        return -1;
    return (int)pgm_read_byte(&speciesTable[index].totalDays);
    //return (int)pgm_read_word(&speciesTable[index].totalDays);
}

uint8_t SpeciesContext::getTotalDays(Species s)
{
    return getTotalDays(static_cast<uint8_t>(s));
}

uint8_t SpeciesContext::getHatchStartDay(uint8_t index)
{
    if (index >= static_cast<uint8_t>(Species::COUNT))
        return -1;
    return (int)pgm_read_byte(&speciesTable[index].hatchStartDay);
}

uint8_t SpeciesContext::getHatchStartDay(Species s)
{
    return getHatchStartDay(static_cast<uint8_t>(s));
}

uint16_t SpeciesContext::getHatchHumi(uint8_t index)
{
    if (index >= static_cast<uint8_t>(Species::COUNT))
        return -1;
    return (uint16_t)pgm_read_word(&speciesTable[index].hatchHumi);
}

uint16_t SpeciesContext::getHatchHumi(Species s)
{
    return getHatchHumi(static_cast<uint8_t>(s));
}

// 특정 종의 프로필을 가져옵니다.
const SpeciesProfile SpeciesContext::getProfile(Species species)
{
    return getProfile(static_cast<uint8_t>(species));
}

const SpeciesProfile SpeciesContext::getProfile(uint8_t index)
{
    SpeciesProfile result; // RAM에 임시 저장할 그릇 생성
    if (index >= static_cast<uint8_t>(Species::COUNT))
    {
        index = 0;
    }
    // Flash(speciesTable[index])에서 RAM(&result)으로 구조체 크기만큼 복사
    memcpy_P(&result, &speciesTable[index], sizeof(SpeciesProfile));

    return result;
}

// 부화 설정을 특정 종의 기본값으로 초기화합니다.
void SpeciesContext::getConfigOfSpecies(SystemConfig &config, Species species)
{
    const SpeciesProfile &profile = getProfile(species);
    config.selectedSpecies = species;
    config.targetTemp = profile.targetTemp;
    config.targetHumi = profile.targetHumi;
    config.turnInterval = (uint32_t)profile.turnInterval * 60; // 분 -> 초 변환
    config.incubationStartTime = 0;                            // 시작 시각은 리셋
}
