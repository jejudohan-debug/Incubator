#include "RtcControl.h"

void RtcControl::handleAction(SystemAction action)
{
    switch (action)
    {
    case SystemAction::TIME_CHANGE:
        if (_view.getSpecies() == Species::BROODER)
        {
            calculateBrooderTemp();
        }
        else
        {
            turnStrategy();
        }
        break;
    case SystemAction::LIMIT_SW:
        if (_view.getSpecies() != Species::BROODER)
        {
            _isLimitReached = true;
            turnStrategy();
        }
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
    if (_operate.getManualTurn())
        return;

    auto& speciesCtx = SystemContext::getInstance().getSpeciesContext();
    Species currentSpecies = _view.getSpecies();
    //uint8_t totalDays = speciesCtx.getTotalDays(currentSpecies);
    uint8_t hatchStartDay = speciesCtx.getHatchStartDay(currentSpecies);

    if (_view.getElapsedDay() >= hatchStartDay) return;
        
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

    if (_operate.getRelayTurn() != shouldTurn)
    {
        _operate.setRelayTurn(shouldTurn);
        _view.updateRelayFlag(UpdateFlag::RELAY_TURN);
    }
}

void RtcControl::calculateBrooderTemp()
{
    static int lastUpdateWeek = -1;

    if (_view.getPageStep() == PageStep::DAY)
        return;

    int currentWeek = _view.getElapsedDay() / 7;

    if (lastUpdateWeek != -1 && currentWeek != lastUpdateWeek)
    {
        uint16_t temp = _view.getTargetTempFixed() - WEEKLY_DROP;
        _view.setTargetTemp(temp);
        _cfgEEPROM.importViewConfigValue();
    }
    lastUpdateWeek = currentWeek;
}
