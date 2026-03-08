#include "NormalControl.h"

void NormalControl::handleAction(SystemAction action)
{
    if (action >= SystemAction::GO_SETUP && action <= SystemAction::MOTOR_STOP)
    {
        int index = static_cast<int>(action) - static_cast<int>(SystemAction::GO_SETUP);
        const int mapSize = sizeof(_normalActionMap) / sizeof(_normalActionMap[0]);
        if (index >= 0 && index < mapSize)
        {
            ActionHandler handler = _normalActionMap[index];
            if (handler)
            {
                (this->*handler)();
            }
        }
    }
}

void NormalControl::goSetupMode()
{
    _view.mode = SystemModes::SETUP;
    _view.setup.isWaitingConfirm = false;
    _view.setup.step = SetupStep::SPECIES;
    _view.isUpdated = true;
}

void NormalControl::displayPageEnv()
{
    _view.normal.page = NormalPage::ENV;
    _view.isUpdated = true;
}

void NormalControl::displayPageRelays()
{
    _view.normal.page = NormalPage::RELAYS;
    _view.isUpdated = true;
}

void NormalControl::displayPageConfig()
{
    _view.normal.page = NormalPage::CONFIG;
    _view.isUpdated = true;
}

void NormalControl::startMotor()
{
    // TODO: motor start logic (enqueue action)
}

void NormalControl::stopMotor()
{
    // TODO: motor stop logic (enqueue action)
}
