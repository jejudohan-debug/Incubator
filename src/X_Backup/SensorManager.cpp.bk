#include "SensorManager.h"

void SensorManager::init() {
    _dhtSensor.init();
}

void SensorManager::update() {
    _dhtSensor.update();

    float temp = _dhtSensor.getTemp();
    float humi = _dhtSensor.getHumi();

    if (_view.normal.currentTemp != temp) {
        _view.normal.currentTemp = temp;
        _view.isUpdated = true;
        _queue.push(SystemAction::TEMP_CHANGE);
    }
    if (_view.normal.currentHumi != humi) {
        _view.normal.currentHumi = humi;
        _view.isUpdated = true;
        _queue.push(SystemAction::HUMI_CHANGE);
    }
}  
