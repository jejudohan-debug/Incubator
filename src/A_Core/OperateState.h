#pragma once
#include <Arduino.h>
#include "CoreTypes.h"

class OperateState
{
private:
    OperateStateFlag::Type _operateStateFlag = OperateStateFlag::NONE;

    //    void setOperateFlag(OperateStateFlag::Type oFlag, bool enable, UpdateFlag::Type uFlag);
    void setOperateFlag(OperateStateFlag::Type oFlag, bool enable);

public:
    OperateState() = default;
    OperateState(const OperateState &) = delete;
    OperateState &operator=(const OperateState &) = delete;

    void setRelayHeat(bool val);
    bool getRelayHeat() const;
    void setRelayFan(bool val);
    bool getRelayFan() const;
    void setRelayTurn(bool val);
    bool getRelayTurn() const;

    void setManualHeat(bool val);
    bool getManualHeat() const;
    void setManualFan(bool val);
    bool getManualFan() const;
    void setManualTurn(bool val);
    bool getManualTurn() const;

    void setWaiting(bool val);
    bool getWaiting() const;
    void setAlert(bool val);
    bool getAlert() const;

    /*void setAutoTune(bool val);
    bool getAutoTune() const;
    void setAutoTuneWait(bool val);
    bool getAutoTuneWait() const;*/
};