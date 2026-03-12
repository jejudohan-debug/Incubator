#pragma once
#include <Arduino.h>
#include "A_Core/CoreTypes.h"
#include "A_Core/SystemContext.h"
#include "A_Core/Interfaces.h"

class ConfigEEPROM : public Subject
{
private:
    static constexpr uint16_t MAGIC_SIG = 0xABCD;

    SystemConfig _config;
    const int CONFIG_ADDR = 0;

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

    uint8_t calculateChecksum(const SystemConfig &cfg);

public:
    ConfigEEPROM() = default;

    void init();
    void update();

    bool load();
    void save();
    bool isViewChanged();
    void importViewConfigValue();
    void exportViewConfigValue();
    void resetSpeciesConfig(Species s);
    void resetSpeciesConfig();

    const Species getSpecies() { return _config.selectedSpecies; }
    const uint16_t getTargetTemp() { return _config.targetTemp; }
    const uint16_t getTargetHumi() { return _config.targetHumi; }
    const uint16_t getTurnInterval() { return _config.turnInterval; }
    const uint16_t getTurnDuration() { return _config.turnDuration; }
};