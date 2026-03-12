#include "ActionLogic.h"

void ActionLogic::init()
{
    for (auto *mgr : _mgrs)
    {
        if (mgr)
            mgr->init();
    }
    _cfgEEPROM.init();
    _outManager.init();
    _display.init();

    _view.updateFlags = UpdateFlag::ALL;
}

void ActionLogic::update()
{
    if (Wire.getWireTimeoutFlag())
    {
        _display.init();
        Wire.clearWireTimeoutFlag();
    }
    for (auto *mgr : _mgrs)
    {
        if (mgr)
            mgr->update();
    }

    while (!_actionQueue.isEmpty())
    {
        SystemAction action = _actionQueue.pop();
        if (action == SystemAction::NONE)
            continue;

        // int index = static_cast<int>(action);
        if (action <= SystemAction::BUTTON_LAST)
        {
            _inputControl.handleAction(action);
        }
        else if (action <= SystemAction::SENSOR_LAST)
        {
            _ssorControl.handleAction(action);
        }
        else if (action < SystemAction::COUNT)
        {
            _rtcControl.handleAction(action);
        }
    }

    _cfgEEPROM.update();
    _outManager.update();
    _display.update(); // LCD 갱신

    _view.updateFlags = UpdateFlag::NONE;
}
