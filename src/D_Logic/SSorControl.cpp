#include "SSorControl.h"

void SSorControl::handleAction(SystemAction action)
{
    switch (action)
    {
    case SystemAction::TEMP_CHANGE:
        updateHeating();
        break;
    case SystemAction::HUMI_CHANGE:
        humiStrategy();
        break;
    default:
        break;
    }

    if (!_operate.getManualFan())
    {
        uint16_t currentTemp = _view.getCurrentTempFixed();
        uint16_t currentHumi = _view.getCurrentHumiFixed();
        exhaustStrategy(currentTemp, currentHumi);
    }
}

void SSorControl::updateHeating()
{
    int16_t nextOutput;

    if (_operate.getManualHeat()) {
        nextOutput = 1000;
    } else {
        nextOutput = computeIntegerPID();
    }
    
    _operate.setHeatOutput(nextOutput);
}

    // int Kp = 2000;
    // int Ki = 15;
    // int Kd = 500;
uint16_t SSorControl::computeIntegerPID()
{
    int32_t currentTemp = _view.getCurrentTempFixed();
    int32_t targetTemp = _view.getTargetTempFixed();
    int32_t error = targetTemp - currentTemp;

    int32_t pTerm = (int32_t)_view.getPID_Kp() * error;

    int32_t dTerm = (int32_t)_view.getPID_Kd() * (error - _lastError);
    _lastError = error;

    if (abs(error) < 20) 
    {
        _integral += error;
        
        if (_integral > 1000) _integral = 1000; 
        else if (_integral < -1000) _integral = -1000;
    }
    else 
    {
        _integral = 0; // 멀리 있을 때는 적분항 초기화 (Windup 방지)
    }

    long iTerm = (int32_t)_view.getPID_Ki() * _integral;

    // --- 4. 최종 출력 계산 ---
    long totalOutput = (pTerm + iTerm + dTerm) / 100;

    if (totalOutput >= 1000) 
    {
        // 출력이 이미 최대라면 적분값을 더 이상 늘리지 않음 (Windup 방지 핵심)
        if (error > 0) _integral -= error; 
        return 1000;
    }
    
    if (totalOutput <= 0) 
    {
        if (error < 0) _integral -= error;
        return 0;
    }

    return (uint16_t)totalOutput;
}

    /* if (abs(error) < 20) 
    {
        _integral += error;
        
        if (_integral > 500) _integral = 500; 
        else if (_integral < -500) _integral = -500;
    }
    else 
    {
        _integral = 0; // 멀리 있을 때는 적분항 초기화 (Windup 방지)
    }

    int32_t pTerm = (int32_t)_view.getPID_Kp() * error;

    long iTerm = (int32_t)_view.getPID_Ki() * _integral;

    int32_t dTerm = (int32_t)_view.getPID_Kd() * (error - _lastError);

    long totalOutput = (pTerm + iTerm + dTerm) / 100;
    if (totalOutput >= 1000) return 1000;
    if (totalOutput <= 0) return 0;

    _lastError = error;
    return (uint16_t)totalOutput;
}*/

void SSorControl::humiStrategy()
{
    if (_operate.getManualHumi())
        return;

    const uint16_t HUMI_GAP = 15;
    // const unsigned long minOnTime = 2000;

    uint16_t currentHumi = _view.getCurrentHumiFixed();
    uint16_t targetHumi = _view.getTargetHumiFixed();

    if (currentHumi < (targetHumi - HUMI_GAP))
    {
        // "가습기를 켜고 싶어" (실제 켜질지는 RelayActuator가 판단)
        if (!_operate.getRelayHumi()) {
            _operate.setRelayHumi(true);
            _view.updateRelayFlag(UpdateFlag::RELAY_HUMI);
        }
    }
    else if (currentHumi >= targetHumi)
    { // 오버슈트 방지를 위해 목표치에서 바로 끄는 것을 권장
        // "가습기를 끄고 싶어"
        if (_operate.getRelayHumi()) {
            _operate.setRelayHumi(false);
            _view.updateRelayFlag(UpdateFlag::RELAY_HUMI);
        }
    }
}

void SSorControl::exhaustStrategy(uint16_t currentTemp, uint16_t currentHumi)
{
    const uint16_t TEMP_OVER_GAP = 10;  // 0.5도 상한
    //const uint16_t HUMI_OVER_GAP = 30; // 3.0% 상한

    uint16_t targetTemp = _view.getTargetTempFixed();
    //uint16_t targetHumi = _view.getTargetHumiFixed();

    bool startExhaust = (currentTemp > targetTemp + TEMP_OVER_GAP); //||
    //                    (currentHumi > targetHumi + HUMI_OVER_GAP);

    //bool stopExhaust = (currentTemp <= targetTemp) &&
    //                   (currentHumi <= targetHumi);

    if (startExhaust) {
        if (!_operate.getRelayFan()) {
            _operate.setRelayFan(true);
            _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
        }
    } else if (_operate.getRelayFan()) {
        _operate.setRelayFan(false);
        _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
    }
    /*else if (stopExhaust) {
        if (_operate.getRelayFan()) {
            _operate.setRelayFan(false);
            _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
        }
    }*/
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