#pragma once

#include <Arduino.h>
#include "CoreTypes.h"

/**
 * @class SpeciesManager
 * @brief 종별 기본 부화 데이터(온도, 습도, 전란 주기 등)를 관리합니다.
 */
class SpeciesContext {
private:
    static const SpeciesProfile speciesTable[static_cast<uint8_t>(Species::COUNT)];

public:
    // 특정 종의 이름을 문자열로 가져옵니다.
    static const char* getSpeciesName(Species species);
    static const char* getSpeciesName(uint8_t index);
    
    // 특정 종의 프로필을 가져옵니다.
    static const SpeciesProfile& getProfile(Species species);
    static const SpeciesProfile& getProfile(uint8_t index);

    // 부화 설정을 특정 종의 기본값으로 초기화합니다.
    static void applyDefaults(SystemConfig& config, Species species);

    // 이름으로 종 인덱스 찾기 (웹 UI 등에서 사용)
    static Species findByName(const char* name);
};
