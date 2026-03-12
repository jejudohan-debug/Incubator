#include "OperateState.h"

/*void OperateState::setOperateFlag(OperateStateFlag::Type oFlag, bool enable, UpdateFlag::Type uFlag)
{
    if (enable)
        _operateStateFlag |= oFlag; // 비트 켜기 (OR)
    else
        _operateStateFlag &= ~oFlag; // 비트 끄기 (AND NOT)
    updateFlags |= uFlag;
}*/

void OperateState::setOperateFlag(OperateStateFlag::Type oFlag, bool enable)
{
    if (enable)
        _operateStateFlag |= oFlag; // 비트 켜기 (OR)
    else
        _operateStateFlag &= ~oFlag; // 비트 끄기 (AND NOT)
}

void OperateState::setRelayHeat(bool val)
{
    if (getRelayHeat() == val)
        return;

    setOperateFlag(OperateStateFlag::HEAT, val);
}

void OperateState::setManualHeat(bool val)
{
    setOperateFlag(OperateStateFlag::M_HEAT, val);
    setRelayHeat(val);
}

bool OperateState::getRelayHeat() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::HEAT);
}

bool OperateState::getManualHeat() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::M_HEAT);
}

void OperateState::setRelayFan(bool val)
{
    if (getRelayFan() == val)
        return;

    setOperateFlag(OperateStateFlag::FAN, val);
}

void OperateState::setManualFan(bool val)
{
    setOperateFlag(OperateStateFlag::M_FAN, val);
    setRelayFan(val);
}

bool OperateState::getRelayFan() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::FAN);
}

bool OperateState::getManualFan() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::M_FAN);
}

void OperateState::setRelayTurn(bool val)
{
    if (getRelayTurn() == val)
        return;

    setOperateFlag(OperateStateFlag::TURN, val);
}

void OperateState::setManualTurn(bool val)
{
    setOperateFlag(OperateStateFlag::M_TURN, val);
    setRelayTurn(val);
}

bool OperateState::getRelayTurn() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::TURN);
}

bool OperateState::getManualTurn() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::M_TURN);
}

void OperateState::setWaiting(bool val)
{
    if (getWaiting() == val)
        return;

    setOperateFlag(OperateStateFlag::WAITING, val);
}

bool OperateState::getWaiting() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::WAITING);
}

void OperateState::setAlert(bool val)
{
    if (getAlert() == val)
        return;

    setOperateFlag(OperateStateFlag::ALERT, val);
}

bool OperateState::getAlert() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::ALERT);
}

/*void OperateState::setNeedSave(bool val)
{
    if (getNeedSave() == val)
        return;

    setOperateFlag(OperateStateFlag::NEED_SAVE, val);
}

bool OperateState::getNeedSave() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::NEED_SAVE);
}

void OperateState::setNeedLoad(bool val)
{
    if (getNeedLoad() == val)
        return;

    setOperateFlag(OperateStateFlag::NEED_LOAD, val);
}

bool OperateState::getNeedLoad() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::NEED_LOAD);
}*/