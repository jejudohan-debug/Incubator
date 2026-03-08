#pragma once
#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"

class SetupControl : public IControl
{
private:
    DisplayState &_view = SystemContext::getView();
    SystemConfig &_config = SystemContext::getConfig();
    SystemConfig _tempConfig = _config;

    using ActionHandler = void (SetupControl::*)();
    ActionHandler _setupActionMap[7] = {
        &SetupControl::endSetupMode,
        &SetupControl::moveToNextStep,
        &SetupControl::increaseValue,
        &SetupControl::decreaseValue,
        &SetupControl::waitConfirm,
        &SetupControl::confirmSave,
        &SetupControl::cancelSave};

    using IncreaseHandler = void (SetupControl::*)();
    using DecreaseHandler = void (SetupControl::*)();
    IncreaseHandler _increaseHandlerMap[7] = {
        &SetupControl::increaseSpecies,
        &SetupControl::increaseDay,
        &SetupControl::increaseHour,
        &SetupControl::increaseMinute,
        &SetupControl::increaseTemperature,
        &SetupControl::increaseHumidity,
        &SetupControl::increaseTurnInterval};
    DecreaseHandler _decreaseHandlerMap[7] = {
        &SetupControl::decreaseSpecies,
        &SetupControl::decreaseDay,
        &SetupControl::decreaseHour,
        &SetupControl::decreaseMinute,
        &SetupControl::decreaseTemperature,
        &SetupControl::decreaseHumidity,
        &SetupControl::decreaseTurnInterval};

    void endSetupMode();
    void moveToNextStep();
    void increaseValue();
    void decreaseValue();
    void waitConfirm();
    void confirmSave();
    void cancelSave();

    // increase/decrease helpers
    void increaseSpecies();
    void increaseDay();
    void increaseHour();
    void increaseMinute();
    void increaseTemperature();
    void increaseHumidity();
    void increaseTurnInterval();

    void decreaseSpecies();
    void decreaseDay();
    void decreaseHour();
    void decreaseMinute();
    void decreaseTemperature();
    void decreaseHumidity();
    void decreaseTurnInterval();

public:
    void init() override;
    void update() override;
    void handleAction(SystemAction action) override;
};
