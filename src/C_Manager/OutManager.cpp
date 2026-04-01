#include "OutputManager.h"

void OutputManager::init()
{
    for (auto &relay : _relays)
    {
        relay.init();
    }
    _ssr.init();
}

/*void OutputManager::switchingRelay(int index, UpdateFlag::Type flag, bool isOn)
{
    if (!UpdateFlag::hasFlag(_view.updateFlag, flag)) return;
}*/

void OutputManager::update()
{
    for (const auto &map : _relayMaps)
    {
        if (UpdateFlag::hasFlag(_view.updateFlags, map.flag))
        {
            bool isOn = (_operate.*(map.getter))();

            isOn ? _relays[map.index].run() : _relays[map.index].stop();
        }
    }

    if (UpdateFlag::hasFlag(_view.updateFlags, UpdateFlag::RELAY_HEAT))
    {
        int16_t nextOutput;
        // if (_operate.getAutoTune())
        //     nextOutput = autoTunePID();
        // else
        nextOutput = computeIntegerPID();

        _ssr.setOutput(nextOutput);
    }
    _ssr.update();
}

int16_t OutputManager::computeIntegerPID()
{
    // int Kp = 2000;
    // int Ki = 15;
    // int Kd = 500;

    int32_t currentTemp = _view.getCurrentTempFixed();
    int32_t targetTemp = _view.getTargetTempFixed();
    int32_t error = targetTemp - currentTemp;

    // --- 1. 변화율(slope) 계산 (PID 계산 전에 수행) ---
    static int32_t _lastTemp = currentTemp;
    static bool isRising = false;

    static int32_t _lastOutput = 0; /* - */
    Serial.print(F(">> lastTemp "));
    Serial.print(_lastTemp);
    Serial.print(F(" >> lastOutput "));
    Serial.println(_lastOutput);

    if (currentTemp > _lastTemp)
    {
        isRising = true;
    }
    else if (currentTemp < _lastTemp)
    {
        isRising = false;
    }
    _lastTemp = currentTemp; // 다음 루프를 위해 저장

    int32_t cutoffThreshold = isRising ? 30 : 5;
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

    Serial.print(F(">> Temp "));
    Serial.println(currentTemp);
    Serial.print(F(">> output "));
    Serial.print(finalOutput);
    Serial.print(F(" = pTerm("));
    Serial.print(pTerm);
    Serial.print(F(") + ("));
    Serial.print(iTerm);
    Serial.print(F(") + ("));
    Serial.print(dTerm);
    Serial.print(F("), integral = "));
    Serial.println(integral);

    _lastOutput = finalOutput; //* - */
    return finalOutput;        // 0 ~ 1000 사이의 값 반환
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