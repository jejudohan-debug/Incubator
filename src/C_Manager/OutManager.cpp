#include "OutputManager.h"

const RelayMap OutputManager::_relayMaps[RELAY_CNT] PROGMEM = {
    {FAN, UpdateFlag::RELAY_FAN, &OperateState::getRelayFan,
     &OperateState::getStateFan, &OperateState::setStateFan, 10000, 20000},
    {TURN, UpdateFlag::RELAY_TURN, &OperateState::getRelayTurn,
     &OperateState::getStateTurn, &OperateState::setStateTurn, 2000, 3000},
    {HUMIDIFIER, UpdateFlag::RELAY_HUMI, &OperateState::getRelayHumi,
     &OperateState::getStateHumi, &OperateState::setStateHumi, 2000, 3000}};

void OutputManager::init()
{
    RelayMap tempMap;
    for (uint8_t i = 0; i < RELAY_CNT; ++i)
    {
        memcpy_P(&tempMap, &(_relayMaps[i]), sizeof(RelayMap));
        _relays[tempMap.index].init();
        _relays[tempMap.index].setMinTimes(tempMap.minOnTime, tempMap.minOffTime);
    }
    _ssr.init();
}

void OutputManager::update()
{
    updateAllRelays();

    updateHeatingSSR();
}

void OutputManager::updateAllRelays()
{
    for (uint8_t i = 0; i < RELAY_CNT; ++i)
    {
        RelayMap map;
        memcpy_P(&map, &_relayMaps[i], sizeof(RelayMap));
        if (_view.hasUpdateFlag(map.flag))
        {
            bool targetState = (_operate.*(map.getter))(); // 릴레이 동작 결정 상태
            _relays[map.index].setOutput(targetState);
        }
        _relays[map.index].update();                          // 실제 동작 상태 업데이트
        bool currentRunning = _relays[map.index].isRunning(); // 실제 릴레이 상태
        if ((_operate.*(map.statelGet))() != currentRunning)
        {
            (_operate.*(map.stateSet))(currentRunning);
            _view.updateRelayFlag(map.flag);
        }
    }
}

void OutputManager::updateHeatingSSR()
{
    _ssr.setOutput(_operate.getHeatOutput());
    _ssr.update();
    
    bool currentRunning = _ssr.isRunning();
    if (_operate.getStateHeat() != currentRunning)
    {
        _operate.setStateHeat(currentRunning);
        _view.updateRelayFlag(UpdateFlag::RELAY_HEAT);
    }
}

/*int16_t OutputManager::computeIntegerPID()
{
    // int Kp = 2000;
    // int Ki = 15;
    // int Kd = 500;

    int32_t currentTemp = _view.getCurrentTempFixed();
    int32_t targetTemp = _view.getTargetTempFixed();
    int32_t error = targetTemp - currentTemp;

    // --- 1. 변화율(slope) 계산 (PID 계산 전에 수행) ---
    static int32_t _lastTemp = currentTemp;
    int32_t slope = currentTemp - _lastTemp;
    _lastTemp = currentTemp; // 다음 루프를 위해 저장

    int32_t cutoffThreshold = (slope > 0) ? 30 : 5;
    if (error <= cutoffThreshold)
    {
        integral = 0; // 적분항 초기화하여 오버슈트 방지
        lastError = error;
        return 0;
    }

    int32_t pTerm = (int32_t)_view.getPID_Kp() * error;
    long potentialIntegral = integral + (long)error;
    long iTerm = (int32_t)_view.getPID_Ki() * potentialIntegral;
    int32_t dTerm = (int32_t)_view.getPID_Kd() * (error - lastError);

    long totalOutput = (pTerm + iTerm + dTerm) / 100;

    int finalOutput;
        if (totalOutput >= 1000)
    {
        finalOutput = 1000;
    }
    else if (totalOutput <= 0)
    {
        finalOutput = 0;
    }
    else
    {
        finalOutput = (int)totalOutput;
        integral = potentialIntegral; // 범위 내일 때만 실제 적분값 업데이트
    }

    lastError = error;

    Serial.print(F(">> Temp ")); Serial.print(currentTemp);
    Serial.print(F(">> output ")); Serial.print(finalOutput);
    Serial.print(F(" = pTerm(")); Serial.print(pTerm);
    Serial.print(F(") + (")); Serial.print(iTerm);
    Serial.print(F(") + (")); Serial.print(dTerm);
    Serial.print(F("), integral = ")); Serial.println(integral);

    return finalOutput; // 0 ~ 1000 사이의 값 반환
}

int16_t OutputManager::autoTunePID()
{
    int16_t nextOutput;

    int32_t currentTemp = _view.getCurrentTempFixed();
    int32_t targetTemp = _view.getTargetTempFixed();

    if (_autotuner.isStarted)
    {
        nextOutput = _autotuner.update(currentTemp);
    }
    else if (currentTemp >= (targetTemp - 10))
    {
        _autotuner.start(targetTemp);
        _autotuner.isStarted = true;
        nextOutput = _autotuner.update(currentTemp);
    }
    else
    {
        nextOutput = 1000;
    }

    if (_autotuner.isComplete) {
        _view.setPID_Kp(_autotuner.Kp);
        _view.setPID_Ki(_autotuner.Ki);
        _view.setPID_Kd(_autotuner.Kd);

        integral = 0;
        lastError = 0;

        _operate.setAutoTune(false);
        _autotuner.isStarted = false;

        _actionQueue.push(SystemAction::CFG_SAVE);
    }
    return nextOutput;
}*/