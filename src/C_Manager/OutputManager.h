#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwarePins.h"
#include "B_Hardware/RelayActuator.h"
#include "B_Hardware/PID_SSR_Actuator.h"
// #include "PIDAutotuner.h"

enum RelayIdx
{
    // HEAT = 0,
    FAN = 0,
    TURN = 1,
    HUMIDIFIER = 2,
    COUNT
};

struct RelayMap
{
    RelayIdx index;
    UpdateFlag::Type flag;
    bool (OperateState::*getter)() const;
    bool (OperateState::*statelGet)() const;
    void (OperateState::*stateSet)(bool);
    uint16_t minOnTime;  // ms 단위
    uint16_t minOffTime; // ms 단위
};

class OutputManager
{
private:
    RelayActuator::Group<RELAY_CNT> &_relays;
    PID_SSR_Actuator _ssr;
    // PIDAutotuner _autotuner;

    static const RelayMap _relayMaps[RELAY_CNT] PROGMEM;

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

    void updateAllRelays();
    void updateHeatingSSR();
    // int16_t autoTunePID();

public:
    OutputManager(RelayActuator::Group<RELAY_CNT> &relays, PID_SSR_Actuator &ssr)
        : _relays(relays), _ssr(ssr) {}

    void init();
    void update();
};