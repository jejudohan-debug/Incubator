#include "SensorControl.h"

SensorControl::SensorControl() {}

void SensorControl::init()
{
    _sensor.init();
}

void SensorControl::update()
{
    _sensor.update();
}

void SensorControl::handleAction(SystemAction action)
{
    switch (action) {
        case SystemAction::TEMP_CHANGE:
            tempStrategy();
            break;
        case SystemAction::HUMI_CHANGE:
            humiStrategy();
            break;
        default:
            break;
    }
}

void SensorControl::tempStrategy() {

}

void SensorControl::humiStrategy() {

}