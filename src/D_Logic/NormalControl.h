#pragma once

#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"

class NormalControl : public IControl
{
private:
    DisplayState &_view = SystemContext::getView();
    // SystemConfig &_config;

    using ActionHandler = void (NormalControl::*)();
    ActionHandler _normalActionMap[6] = {
        &NormalControl::goSetupMode,
        &NormalControl::displayPageEnv,
        &NormalControl::displayPageRelays,
        &NormalControl::displayPageConfig,
        &NormalControl::startMotor,
        &NormalControl::stopMotor};

    void goSetupMode();
    void displayPageEnv();
    void displayPageRelays();
    void displayPageConfig();
    void startMotor();
    void stopMotor();

public:
    void init() override {}
    void update() override {};
    void handleAction(SystemAction action) override;
};
