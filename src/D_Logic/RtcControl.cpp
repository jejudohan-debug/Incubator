#include "RtcControl.h"

void RtcControl::handleAction(SystemAction action)
{
    switch (action)
    {
    case SystemAction::TIME_CHANGE:
        turnStrategy();
        break;
    case SystemAction::LIMIT_SW:
        _isLimitReached = true;
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
    if (_view.getManualTurn())
        return;

    uint16_t turnInterval = _view.getTurnInterval() * 60; // 초 단위 변환
    uint16_t turnDuration = _view.getTurnDuration();      // 초 단위

    if (turnInterval == 0)
        return;

    uint32_t eTime = _view.getElapsedUnixTime() % turnInterval;
    bool timeToTurn = (eTime < turnDuration);

    if (!timeToTurn)
        _isLimitReached = false; // 대기 시간 동안 플래그 초기화

    // [핵심] 주기가 시작된 직후 2초 동안은 스위치가 눌려도 무시 (Effective Limit 계산)
    // 모터가 확실히 스위치 범위를 벗어날 시간을 벌어줍니다.
    bool ignoreLimit = (eTime < 2); 
    bool currentLimit = ignoreLimit ? false : _isLimitReached;

    bool shouldTurn = timeToTurn && !currentLimit;

    if (_view.getRelayTurn() != shouldTurn)
        _view.setRelayTurn(shouldTurn);
}
