#include "EventSSorManager.h"

EventSSorManager::EventSSorManager(DHTSensor &dht)
    : _dht(dht) {}

void EventSSorManager::init()
{
    _dht.init();
    _dht.addObserver(this);

    _view.setCurrentTemp(_dht.getTemp());
    _view.setCurrentHumi(_dht.getHumi());
}

void EventSSorManager::update()
{
    _dht.update();
}

void EventSSorManager::onNotify(EventFlag::Type flag, const float value)
{
    switch (flag)
    {
    case EventFlag::DHT_TEMP:
        handleDhtTemp(value);
        break;
    case EventFlag::DHT_HUMI:
        handleDhtHumi(value);
        break;
    default:
        break;
    }
}

void EventSSorManager::handleDhtTemp(const float value)
{
    _view.setCurrentTemp(value);
    _actionQueue.push(SystemAction::TEMP_CHANGE);
}

void EventSSorManager::handleDhtHumi(const float value)
{
    _view.setCurrentHumi(value);
    _actionQueue.push(SystemAction::HUMI_CHANGE);
}
