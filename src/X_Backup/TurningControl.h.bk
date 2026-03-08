#pragma once

#include "A_Core/Interfaces.h"
#include "B_Hardware/HardwareFactory.h"

class TurningControl : IControl {
private:
    SystemConfig& _config = SystemContext::getConfig();
    IActuator* _motor;

    unsigned long _lastTurnTime = 0;
    unsigned long _autoTurnStartTime = 0;
    bool _isAutoTurning = false;
    const unsigned long DURATION = 15000; // 15초

public:
    TurningControl() = default;

    void init() override{}
    void handleAction(SystemAction action) override {}
    void update() override {
        unsigned long now = millis();

        // 자동 전란 타이머 체크
        if (_config.turnInterval > 0) {
            unsigned long intervalMs = (unsigned long)_config.turnInterval * 60000;
            if (!_isAutoTurning && (now - _lastTurnTime >= intervalMs || _lastTurnTime == 0)) {
                _isAutoTurning = true;
                _autoTurnStartTime = now;
                _lastTurnTime = now;
            }
        }

        // 자동 전란 종료 체크
        if (_isAutoTurning && (now - _autoTurnStartTime >= DURATION)) {
            _isAutoTurning = false;
        }

        // 구동 실행 (수동 요청 혹은 자동 전란 중일 때)
        ( _isAutoTurning) ? _motor->run() : _motor->stop();
    }
};