#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwarePins.h"
#include "B_Hardware/RelayActuator.h"
#include "B_Hardware/PID_SSR_Actuator.h"
//#include "PIDAutotuner.h"

enum RelayIdx
{
    // HEAT = 0,
    FAN = 0,
    TURN = 1,
    COUNT
};

class OutputManager
{
private:
    RelayActuator::Group<RELAY_CNT> &_relays;
    PID_SSR_Actuator _ssr;
    //PIDAutotuner _autotuner;

    struct RelayMap
    {
        RelayIdx index;
        UpdateFlag::Type flag;
        bool (OperateState::*getter)() const;
    };
    const RelayMap _relayMaps[RELAY_CNT] = {
        {FAN, UpdateFlag::RELAY_FAN, &OperateState::getRelayFan},
        {TURN, UpdateFlag::RELAY_TURN, &OperateState::getRelayTurn}};
        //{HEAT, UpdateFlag::RELAY_HEAT, &OperateState::getRelayHeat}};

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

        // PID 계산
    int32_t integral = 0;
    int32_t lastError = 0;

    int16_t computeIntegerPID();
    //int16_t autoTunePID();

public:
    OutputManager(RelayActuator::Group<RELAY_CNT> &relays, PID_SSR_Actuator &ssr)
        : _relays(relays), _ssr(ssr) {}

    void init();
    void update();
};