#include "OperateState.h"

#define DEFINE_FLAG_METHODS(Name, Flag)                 \
    void OperateState::set##Name(bool val)              \
    {                                                   \
        updateFlag(OperateStateFlag::Flag, val);        \
    }                                                   \
    bool OperateState::get##Name() const                \
    {                                                   \
        return OperateStateFlag::hasFlag(               \
            _operateStateFlag, OperateStateFlag::Flag); \
    }

bool OperateState::updateFlag(OperateStateFlag::Type flag, bool val)
{
    bool current = OperateStateFlag::hasFlag(_operateStateFlag, flag);
    if (current == val)
        return false; // 변경 사항 없음

    if (val)
        _operateStateFlag |= flag;
    else
        _operateStateFlag &= ~flag;

    return true; // 변경됨
}

void OperateState::setHeatOutput(uint16_t output)
{
    _heatOutput = output;
}

uint16_t OperateState::getHeatOutput() const
{
    return _heatOutput;
}

DEFINE_FLAG_METHODS(RelayHeat, HEAT)
DEFINE_FLAG_METHODS(ManualHeat, M_HEAT)
DEFINE_FLAG_METHODS(StateHeat, STATE_HEAT)

DEFINE_FLAG_METHODS(RelayFan, FAN)
DEFINE_FLAG_METHODS(ManualFan, M_FAN)
DEFINE_FLAG_METHODS(StateFan, STATE_FAN)

DEFINE_FLAG_METHODS(RelayTurn, TURN)
DEFINE_FLAG_METHODS(ManualTurn, M_TURN)
DEFINE_FLAG_METHODS(StateTurn, STATE_TURN)

DEFINE_FLAG_METHODS(RelayHumi, HUMIDIFIER)
DEFINE_FLAG_METHODS(ManualHumi, M_HUMI)
DEFINE_FLAG_METHODS(StateHumi, STATE_HUMI)

// 시스템 상태
DEFINE_FLAG_METHODS(Waiting, WAITING)
DEFINE_FLAG_METHODS(Alert, ALERT)

/*/ 2. 수동 모드 전용 매크로 (Manual 설정 시 Relay도 같이 연동되는 로직)
#define DEFINE_MANUAL_METHODS(Name, MFlag, RFlag)        \
    void OperateState::setManual##Name(bool val)         \
    {                                                    \
        updateFlag(OperateStateFlag::MFlag, val);        \
        setRelay##Name(val);                             \
    }                                                    \
    bool OperateState::getManual##Name() const           \
    {                                                    \
        return OperateStateFlag::hasFlag(                \
            _operateStateFlag, OperateStateFlag::MFlag); \
    }*/

/*void OperateState::setOperateFlag(OperateStateFlag::Type oFlag, bool enable)
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

void OperateState::setRelayHumi(bool val)
{
    if (getRelayHumi() == val)
        return;

    setOperateFlag(OperateStateFlag::HUMIDIFIER, val);
}

bool OperateState::getRelayHumi() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::HUMIDIFIER);
}

void OperateState::setManualHumi(bool val)
{
    setOperateFlag(OperateStateFlag::M_HUMI, val);
    setRelayHumi(val);
}

bool OperateState::getManualHumi() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::M_HUMI);
}

void OperateState::setStateHeat(bool val)
{
    if (getStateHeat() == val)
        return;

    setOperateFlag(OperateStateFlag::STATE_HEAT, val);
}

bool OperateState::getStateHeat() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::STATE_HEAT);
}

void OperateState::setStateFan(bool val)
{
    if (getStateFan() == val)
        return;

    setOperateFlag(OperateStateFlag::STATE_FAN, val);
}

bool OperateState::getStateFan() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::STATE_FAN);
}

void OperateState::setStateTurn(bool val)
{
    if (getStateTurn() == val)
        return;

    setOperateFlag(OperateStateFlag::STATE_TURN, val);
}

bool OperateState::getStateTurn() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::STATE_TURN);
}

void OperateState::setStateHumi(bool val)
{
    if (getStateHumi() == val)
        return;

    setOperateFlag(OperateStateFlag::STATE_HUMI, val);
}

bool OperateState::getStateHumi() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::STATE_HUMI);
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
}*/

/*void OperateState::setAutoTune(bool val)
{
    setOperateFlag(OperateStateFlag::AUTOTUNE, val);
}

bool OperateState::getAutoTune() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::AUTOTUNE);
}

void OperateState::setAutoTuneWait(bool val)
{
    setOperateFlag(OperateStateFlag::AUTOTUNEWAIT, val);
}

bool OperateState::getAutoTuneWait() const
{
    return OperateStateFlag::hasFlag(_operateStateFlag, OperateStateFlag::AUTOTUNEWAIT);
}*/
