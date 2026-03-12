#pragma once

#include <Arduino.h>
#include "CoreTypes.h"

/**
 * @class SpeciesManager
 * @brief 종별 기본 부화 데이터(온도, 습도, 전란 주기 등)를 관리합니다.
 */
class SpeciesContext
{
private:
    static const SpeciesProfile speciesTable[static_cast<uint8_t>(Species::COUNT)];

public:
    SpeciesContext() = default;

    // 복사 금지 (메모리 안정성 확보)
    SpeciesContext(const SpeciesContext &) = delete;
    SpeciesContext &operator=(const SpeciesContext &) = delete;

    // static SpeciesContext &getInstance();
    //  특정 종의 프로필을 가져옵니다.
    const SpeciesProfile getProfile(Species species);
    const SpeciesProfile getProfile(uint8_t index);

    const char *getSpeciesName(Species species);
    const char *getSpeciesName(uint8_t index);
    uint8_t getTotalDays(uint8_t index);
    uint8_t getHatchStartDay(uint8_t index);
    uint8_t getTotalDays(Species s);
    uint8_t getHatchStartDay(Species s);

    // 부화 설정을 특정 종의 기본값으로 초기화합니다.
    void getConfigOfSpecies(SystemConfig &config, Species species);

    // 이름으로 종 인덱스 찾기 (웹 UI 등에서 사용)
    // static Species findByName(const char* name);
};
