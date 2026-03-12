#pragma once
#include <LiquidCrystal_I2C.h>
#include "CoreTypes.h"
#include "ActionQueue.h"
#include "SpeciesContext.h"
#include "DisplayState.h"
#include "OperateState.h"

class SystemContext
{
private:
    SystemContext() = default;

    // 복사 금지 (메모리 안정성 확보)
    SystemContext(const SystemContext &) = delete;
    SystemContext &operator=(const SystemContext &) = delete;

public:
    static SystemContext &getInstance();
    //LiquidCrystal_I2C &getLcd();
    DisplayState &getView();
    OperateState &getOperate();
    ActionQueue &getActionQueue();
    // SystemConfig &getConfig();
    SpeciesContext &getSpeciesContext();
};
