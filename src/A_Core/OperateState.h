#pragma once
#include <Arduino.h>
#include "CoreTypes.h"

class OperateState
{
private:
    OperateStateFlag::Type _operateStateFlag = OperateStateFlag::NONE;

    uint16_t _heatOutput = 0;

    bool updateFlag(OperateStateFlag::Type flag, bool val);

public:
    OperateState() = default;
    OperateState(const OperateState &) = delete;
    OperateState &operator=(const OperateState &) = delete;

    void setHeatOutput(uint16_t output);
    uint16_t getHeatOutput() const;

    void setRelayHeat(bool val);
    bool getRelayHeat() const;
    void setManualHeat(bool val);
    bool getManualHeat() const;
    void setStateHeat(bool val);
    bool getStateHeat() const;
    

    void setRelayFan(bool val);
    bool getRelayFan() const;
    void setManualFan(bool val);
    bool getManualFan() const;
    void setStateFan(bool val);
    bool getStateFan() const;

    void setRelayTurn(bool val);
    bool getRelayTurn() const;
    void setManualTurn(bool val);
    bool getManualTurn() const;
    void setStateTurn(bool val);
    bool getStateTurn() const;

    void setRelayHumi(bool val);
    bool getRelayHumi() const;
    bool getManualHumi() const;
    void setManualHumi(bool val);
    void setStateHumi(bool val);
    bool getStateHumi() const;

    void setWaiting(bool val);
    bool getWaiting() const;
    void setAlert(bool val);
    bool getAlert() const;

    /*void setAutoTune(bool val);
    bool getAutoTune() const;
    void setAutoTuneWait(bool val);
    bool getAutoTuneWait() const;*/
};