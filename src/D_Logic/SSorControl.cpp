#include "SSorControl.h"

void SSorControl::handleAction(SystemAction action)
{
    switch (action)
    {
    case SystemAction::TEMP_CHANGE:
        if (!_operate.getManualHeat())
        {
            _view.updateRelayFlag(UpdateFlag::RELAY_HEAT);
        }
        break;
    case SystemAction::HUMI_CHANGE:
        humiStrategy();
        break;
    default:
        break;
    }
}

void SSorControl::humiStrategy()
{
    if (_operate.getManualFan()) return;
    
    const uint16_t HUMI_GAP = 20;
    uint16_t currentHumi = _view.getCurrentHumiFixed();
    uint16_t targetHumi = _view.getTargetHumiFixed();

    if (!_operate.getRelayFan())
    {
        if (currentHumi > targetHumi + HUMI_GAP)
        {
            _operate.setRelayFan(true);
            _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
        }
    } else {
        if (currentHumi < targetHumi - HUMI_GAP)
        {
            _operate.setRelayFan(false);
            _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
        }
    }
}

void SSorControl::checkHeatHealth()
{
    if (_operate.getRelayHeat())
    {
        // 5분이 지났는데 온도가 시작 시점보다 안 올랐다면?
        if (millis() - _heatingOnTime > 300000)
        {
            if (_view.getCurrentTempFixed() < _startTemp + 1)
            {
                _operate.setAlert(true); // 시스템 중단 및 알람
                _operate.setRelayHeat(false);
                _view.updateRelayFlag(UpdateFlag::RELAY_HEAT);
            }
            else
            {
                _operate.setAlert(false);
            }
        }
    }
}

/*void SensorControl::update()
{
    _dhtSensor.update();

    float temp = _dhtSensor.getTemp();
    float humi = _dhtSensor.getHumi();

    if (_view.normal.currentTemp != temp)
    {
        _view.normal.currentTemp = temp;
        _view.isUpdated = true;
        _queue.push(SystemAction::TEMP_CHANGE);
    }
    if (_view.normal.currentHumi != humi)
    {
        _view.normal.currentHumi = humi;
        _view.isUpdated = true;
        _queue.push(SystemAction::HUMI_CHANGE);
    }
}

void SSorControl::tempStrategy()
{
    
    //const uint16_t TEMP_GAP = 5;
    //const unsigned long RELAY_DELAY = 10000; // 10초
    //unsigned long now = millis();

    //uint16_t currentTemp = _view.getCurrentTempFixed();
    //uint16_t targetTemp = _view.getTargetTempFixed();

    bool isHeating = _operate.getRelayHeat();

    if (!isHeating)
    {
        if ((now - _heatingOffTime >= RELAY_DELAY) &&
            (currentTemp <= targetTemp - TEMP_GAP)) {
            _operate.setRelayHeat(true);
            _view.updateRelayFlag(UpdateFlag::RELAY_HEAT);
            _startTemp = currentTemp;
            _heatingOnTime = now;
        }
    } else {
        if (currentTemp >= targetTemp) {
            _operate.setRelayHeat(false);
            _view.updateRelayFlag(UpdateFlag::RELAY_HEAT);
            _heatingOffTime = now;
        }
    }
}*/