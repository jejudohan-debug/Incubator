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
        UpdateFlag::Type flag;
        bool (DisplayState::*getter)() const;
    };
    const RelayMap _relayMaps[3] = {
        {HEAT, UpdateFlag::R_HEAT, &DisplayState::getRelayHeat},
        {FAN, UpdateFlag::R_FAN, &DisplayState::getRelayFan},
        {TURN, UpdateFlag::R_TURN, &DisplayState::getRelayTurn}};

    DisplayState &_view = SystemContext::getInstance().getView();

public:
    OutputManager(RelayActuator::Group<3> &relays)
        : _relays(relays) {}

    void init();
    void update();
    // void switchingDevice(UpdateFlag::Type flg);
};