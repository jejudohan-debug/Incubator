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

    if (error > 200) {      // 목표보다 2도 이상 낮을 때
        _integral = 0;      // 적분항은 비워둠 (나중에 PID 진입 시 깨끗하게 시작)
        return 1000;        // 무조건 풀가동
    }
    
    if (error < -30) {     // 목표보다 1도 이상 높을 때
        _integral = 0;
        return 0;           // 무조건 끔
    }

    int32_t pTerm = (int32_t)_view.getPID_Kp() * error;

    int32_t dTerm = (int32_t)_view.getPID_Kd() * (error - _lastError);
    _lastError = error;

    if (abs(error) < 50) 
    {
        _integral += error;
        
        if (_integral > 10000) _integral = 10000; 
        else if (_integral < -10000) _integral = -10000;
    }
    else 
    {
        _integral = 0; // 멀리 있을 때는 적분항 초기화 (Windup 방지)
    }

    long iTerm = (int32_t)_view.getPID_Ki() * _integral;

    // --- 4. 최종 출력 계산 ---
    long totalOutput = (pTerm + iTerm + dTerm) / 1000;

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

void SSorControl::humiStrategy()
{
    if (_operate.getManualHumi())
        return;

    const uint16_t HUMI_GAP = 150;

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
    const uint16_t TEMP_OVER_GAP = 100;  // 1도 상한

    uint16_t targetTemp = _view.getTargetTempFixed();

    bool startExhaust = (currentTemp > targetTemp + TEMP_OVER_GAP);

    if (startExhaust) {
        if (!_operate.getRelayFan()) {
            _operate.setRelayFan(true);
            _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
        }
    } else if (_operate.getRelayFan()) {
        _operate.setRelayFan(false);
        _view.updateRelayFlag(UpdateFlag::RELAY_FAN);
    }
}

void SSorControl::checkHeatHealth()
{
    if (_operate.getRelayHeat())
    {
        // 5분이 지났는데 온도가 시작 시점보다 안 올랐다면?
        if (millis() - _heatingOnTime > 300000)
        {
            if (_view.getCurrentTempFixed() < _startTemp + 100)
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
