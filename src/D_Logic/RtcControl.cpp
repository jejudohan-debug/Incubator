#include "RtcControl.h"

void RtcControl::handleAction(SystemAction action)
{
    switch (action)
    {
    case SystemAction::TIME_CHANGE:
        turnStrategy();
        break;
    case SystemAction::CFG_LOAD:
        break;
    case SystemAction::CFG_SAVE:
        break;
    default:
        break;
    }
}

void RtcControl::turnStrategy()
{
    if (_view.getManualTurn()) return;
    
    uint16_t turnInterval = _view.getTurnInterval() * 60; // 초 단위 변환
    uint16_t turnDuration = _view.getTurnDuration();      // 초 단위

    if (turnInterval == 0)
        return;

    uint32_t eTime = _view.getElapsedUnixTime() % turnInterval;

    bool shouldTurn = (eTime < turnDuration);
    if (_view.getRelayTurn() != shouldTurn)
    {
        _view.setRelayTurn(shouldTurn);
        _view.updateFlags |= UpdateFlag::R_TURN;
    }
}
