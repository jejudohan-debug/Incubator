#pragma once

#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"
#include "C_Manager/SensorManager.h"

class EnvControl {
private:
    //IActuator* _heater;
    //IActuator* _fan;
    SensorManager _sensorMgr;
    DisplayState& _view = SystemContext::getView();
    SystemConfig& _config = SystemContext::getConfig();

    const float HYSTERESIS = 0.2;
    const float SAFETY_TEMP_MAX = 40.0;

public:
    EnvControl() = default;
    EnvControl(const EnvControl&) = delete;

    void init() {
        _sensorMgr.init();
    }   
    
    void update() {
        _sensorMgr.update();
        
        //maintain(view.temp, view.humi, config);
    }

    void maintain(float currentTemp, float currentHumi, const SystemConfig& config) {
        bool heaterOn = false;
        bool fanOn = false;

        // 히터 로직 (Clamping: 안전 온도 최과 시 강제 차단)
        if (currentTemp < (config.targetTemp - HYSTERESIS)) {
            heaterOn = true;
        }

        if (currentTemp > SAFETY_TEMP_MAX) {
            heaterOn = false;
            fanOn = true; // 강제 냉각
        } else {
            // 일반 팬 가동 조건: 히터가 켜졌거나, 온습도가 목표보다 높을 때
            if (heaterOn || (currentTemp > config.targetTemp + HYSTERESIS) || (currentHumi > config.targetHumid)) {
                fanOn = true;
            }
        }

        //heaterOn ? _heater->run() : _heater->stop();
        //fanOn ? _fan->run() : _fan->stop();
    }
};