#include "EventSSorManager.h"

EventSSorManager::EventSSorManager(SHTSensor &sht)
    : _sht(sht) {}

void EventSSorManager::init()
{
    _sht.init();
    _sht.addObserver(this);

    _view.setCurrentTempFixed(_sht.getTemp());
    _view.setCurrentHumiFixed(_sht.getHumi());
}

void EventSSorManager::update()
{
    _sht.update();
}

void EventSSorManager::onNotify(EventFlag::Type flag, const uint16_t value)
{
    switch (flag)
    {
    case EventFlag::SHT_TEMP:
        _view.setCurrentTempFixed(value);
        _actionQueue.push(SystemAction::TEMP_CHANGE);
        break;
    case EventFlag::SHT_HUMI:
        _view.setCurrentHumiFixed(value);
        _actionQueue.push(SystemAction::HUMI_CHANGE);
        break;
    default:
        break;
    }
}

