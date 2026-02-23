#include "SpeciesContext.h"

const SpeciesProfile SpeciesContext::speciesTable[static_cast<uint8_t>(Species::COUNT)] = {
    {"Chicken", 21, 18, 37.5, 45.0, 70.0, 180},
    {"Duck",    28, 25, 37.2, 55.0, 80.0, 120},
    {"Goose",   30, 27, 37.0, 55.0, 85.0, 120}, // 거위
    {"Quail",   17, 14, 37.7, 50.0, 75.0,  60},
    {"Brooder", 30,  0, 35.0, 50.0, 50.0,   0} 
};

// 특정 종의 이름을 문자열로 가져옵니다.
const char* SpeciesContext::getSpeciesName(Species species) {
    return getSpeciesName(static_cast<uint8_t>(species));
}

// 특정 종의 이름을 문자열로 가져옵니다.
const char* SpeciesContext::getSpeciesName(uint8_t index) {
    if (index >= static_cast<uint8_t>(Species::COUNT)) {
        return "Unknown";
    }
    return speciesTable[index].name;
}

// 특정 종의 프로필을 가져옵니다.
const SpeciesProfile& SpeciesContext::getProfile(Species species) {
    return getProfile(static_cast<uint8_t>(species));
}

const SpeciesProfile& SpeciesContext::getProfile(uint8_t index) {
    if (index >= static_cast<uint8_t>(Species::COUNT)) {
        index = 0;
    }
    return speciesTable[index];
}

// 부화 설정을 특정 종의 기본값으로 초기화합니다.
void SpeciesContext::applyDefaults(SystemConfig& config, Species species) {
    const SpeciesProfile& profile = getProfile(species);
    config.selectedSpecies = static_cast<uint8_t>(species);
    config.targetTemp = profile.targetTemp;
    config.targetHumid = profile.targetHumi;
    config.turnInterval = (uint32_t)profile.turnInterval * 60; // 분 -> 초 변환
    config.incubationStartTime = 0; // 시작 시각은 리셋
}

// 이름으로 종 인덱스 찾기 (웹 UI 등에서 사용)
Species SpeciesContext::findByName(const char* name) {
    for (uint8_t i = 0; i < static_cast<uint8_t>(Species::COUNT); i++) {
        if (strcmp(speciesTable[i].name, name) == 0) {
            return static_cast<Species>(i);
        }
    }
    return Species::CHICKEN;
}