#include "IncubatorLogic.h"

void IncubatorLogic::init()
{
    _lcdMgr = ManagerFactory::getLcdManager();
    _inputControl = ControlFactory::getInputControl();
    _sensorCtl = ControlFactory::getSensorControl();
    _turningCtl = ControlFactory::getTurningControl();
    
    _inputControl->init();
    _lcdMgr->init();
    _sensorCtl->init();
}

void IncubatorLogic::update()
{
    _inputControl->update();
    _sensorCtl->update();

    while (!_queue.isEmpty())
    {
        SystemAction action = _queue.pop();
        if (action == SystemAction::NONE) continue;

        //int index = static_cast<int>(action);
        if (action <= SystemAction::BUTTON_LAST)
        {
            _inputControl->handleAction(action);
        }
        else if (action <= SystemAction::SENSOR_LAST)
        {
            _sensorCtl->handleAction(action);
        }
        else if (action < SystemAction::COUNT)
        {
            _turningCtl->handleAction(action);
        }
    }
    _lcdMgr->update(); // LCD 갱신
}