#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwarePins.h"
#include "B_Hardware/RelayActuator.h"

#define RELAY_CNT 3

class OutputManager
{
private:
    RelayActuator::Group<3> &_relays;
    struct RelayMap
    {
        RelayIdx index;
        bool (OperateState::*getter)() const;
    };
    const RelayMap _relayMaps[RELAY_CNT] = {
        {HEAT, &OperateState::getRelayHeat},
        {FAN, &OperateState::getRelayFan},
        {TURN, &OperateState::getRelayTurn}};

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();

public:
    OutputManager(RelayActuator::Group<3> &relays)
        : _relays(relays) {}

    void init();
    void update();
    // void switchingDevice(UpdateFlag::Type flg);
};