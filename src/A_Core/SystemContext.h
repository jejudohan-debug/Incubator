#pragma once
#include <LiquidCrystal_I2C.h>
#include "CoreTypes.h"
#include "EventQueue.h"
#include "SpeciesContext.h"

class SystemContext
{
public:
    SystemContext() = default;

    // 복사 금지 (메모리 안정성 확보)
    SystemContext(const SystemContext &) = delete;
    SystemContext &operator=(const SystemContext &) = delete;

    static SystemContext &getInstance();
    static LiquidCrystal_I2C &getLcd();
    static DisplayState &getView();
    static EventQueue &getQueue();
    static SystemConfig &getConfig();
    static SpeciesContext &getSpeciesContext();
};
