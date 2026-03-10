#include "EventBtnManager.h"

void EventBtnManager::init()
{
    for (auto &btn : _btns)
    {
        btn.init();
        btn.addObserver(this);
    }
}

void EventBtnManager::update()
{
    for (auto &btn : _btns)
    {
        btn.update();
    }
}

void EventBtnManager::onNotify(EventFlag::Type flag, const uint8_t value)
{
    ButtonEvent evt = static_cast<ButtonEvent>(value);

    switch (flag)
    {
    case EventFlag::BTN_SELECT:
        handleBtnSelect(evt);
        break;
    case EventFlag::BTN_UP:
        handleBtnUp(evt);
        break;
    case EventFlag::BTN_DOWN:
        handleBtnDown(evt);
        break;
    default:
        break;
    }
}

void EventBtnManager::handleBtnSelect(const ButtonEvent evt)
{
    if (evt == ButtonEvent::CLICK)
    {
        /*if (_view.getPageStep() == PageStep::ENV || _view.getPageStep() == PageStep::CONFIG)
        {
            _actionQueue.push(SystemAction::HEATER_STOP);
            _actionQueue.push(SystemAction::FAN_STOP);
            _actionQueue.push(SystemAction::TURN_STOP);
        }*/
        _actionQueue.push(_view.getWaiting() ? SystemAction::NONE : SystemAction::MOVE_NEXT);
    }
}

void EventBtnManager::handleBtnUp(const ButtonEvent evt)
{
    switch (_view.getPageStep())
    {
    case PageStep::ENV:
        if (evt == ButtonEvent::PRESS)
        {
            _actionQueue.push(SystemAction::TURN_START);
        }
        else
        {
            _actionQueue.push(SystemAction::TURN_STOP);
        }
        break;
    case PageStep::CONFIG:
        if (evt == ButtonEvent::PRESS)
        {
            _actionQueue.push(SystemAction::HEATER_START);
        }
        else
        {
            _actionQueue.push(SystemAction::HEATER_STOP);
        }
        break;
    default:
        if (evt == ButtonEvent::CLICK)
        {
            _actionQueue.push(_view.getWaiting() ? SystemAction::SAVE_YES : SystemAction::VALUE_UP);
        }
        break;
    }
}

void EventBtnManager::handleBtnDown(const ButtonEvent evt)
{
    switch (_view.getPageStep())
    {
    case PageStep::ENV:
        if (evt == ButtonEvent::PRESS)
        {
            _actionQueue.push(SystemAction::FAN_START);
        }
        else
        {
            _actionQueue.push(SystemAction::FAN_STOP);
        }
        break;
    case PageStep::CONFIG:
        break;
    default:
        if (evt == ButtonEvent::CLICK)
        {
            _actionQueue.push(_view.getWaiting() ? SystemAction::SAVE_NO : SystemAction::VALUE_DOWN);
        }
        break;
    }
}

void EventBtnManager::handleLimitSw(const ButtonEvent evt)
{

    if (evt == ButtonEvent::PRESS)
    {
        _actionQueue.push(SystemAction::LIMIT_SW);
    }
}