#include "ButtonManager.h"
#include <Arduino.h>

// 하드웨어 초기화 (setup에서 호출)
void ButtonManager::init()
{
    for (int i = 0; i < static_cast<int>(BtnIdx::COUNT); i++)
    {
        _btns[i].init();
    }
}

// 매 루프마다 호출
void ButtonManager::update()
{
    for (int i = 0; i < static_cast<int>(BtnIdx::COUNT); i++)
    {
        _btns[i].update();

        ButtonEvent evt = _btns[i].pullEvent();
        if (evt != ButtonEvent::NONE)
        {
            handleEvent(static_cast<BtnIdx>(i), evt);
        }
    }
}

void ButtonManager::handleEvent(BtnIdx idx, ButtonEvent evt)
{
    if (evt == ButtonEvent::NONE)
        return;

    Serial.print("InputEvent: Btn=");
    Serial.print((int)idx);
    Serial.print(" Evt=");
    Serial.println((int)evt);

    switch (_view.mode)
    {
    case SystemModes::NORMAL:
        handleNormalMode(idx, evt);
        break;
    case SystemModes::SETUP:
        handleSetupMode(idx, evt);
        break;
    default:
        break;
    }
}
// Normal 모드용 함수 포인터 테이블 정의
const ButtonManager::NormalEventFunc ButtonManager::_normalFuncTable[] = {
    &ButtonManager::normal_SELECT,
    &ButtonManager::normal_UP,
    &ButtonManager::normal_DOWN,
    &ButtonManager::normal_ENTER,
};

// Setup 모드용 함수 포인터 테이블 정의
const ButtonManager::SetupEventFunc ButtonManager::_setupFuncTable[] = {
    &ButtonManager::setup_SELECT,
    &ButtonManager::setup_UP,
    &ButtonManager::setup_DOWN,
    &ButtonManager::setup_ENTER,
};

void ButtonManager::handleNormalMode(BtnIdx idx, ButtonEvent evt)
{
    int i = static_cast<int>(idx);
    if (i < 0 || i >= static_cast<int>(BtnIdx::COUNT))
        return;
    (this->*_normalFuncTable[i])(evt);
}

void ButtonManager::handleSetupMode(BtnIdx idx, ButtonEvent evt)
{
    int i = static_cast<int>(idx);
    if (i < 0 || i >= static_cast<int>(BtnIdx::COUNT))
        return;
    (this->*_setupFuncTable[i])(evt);
}

// 각 버튼별 Normal Mode 핸들러 구현
void ButtonManager::normal_SELECT(ButtonEvent evt)
{
    if (evt == ButtonEvent::LONG_PRESS)
    {
        Serial.println("Action: GO_SETUP");
        _queue.push(SystemAction::GO_SETUP);
    }
    else if (evt == ButtonEvent::CLICK)
    {
        _queue.push(SystemAction::SHOW_PAGE_ENV);
    }
}

void ButtonManager::normal_UP(ButtonEvent evt)
{
    if (evt == ButtonEvent::CLICK)
    {
        _queue.push(SystemAction::SHOW_PAGE_RELAYS);
    }
}

void ButtonManager::normal_DOWN(ButtonEvent evt)
{
    if (evt == ButtonEvent::CLICK)
    {
        _queue.push(SystemAction::SHOW_PAGE_CONFIG);
    }
}

void ButtonManager::normal_ENTER(ButtonEvent evt)
{
    if (_view.mode == SystemModes::NORMAL)
    {
        if (evt == ButtonEvent::PRESS)
        {
            _queue.push(SystemAction::MOTOR_START);
        }
        if (evt == ButtonEvent::CLICK)
        {
            _queue.push(SystemAction::MOTOR_STOP);
        }
    }
}

void ButtonManager::normal_NOP(ButtonEvent evt) { (void)evt; }

// 각 버튼별 Setup Mode 핸들러 구현
void ButtonManager::setup_SELECT(ButtonEvent evt)
{
    bool waiting = _view.setup.isWaitingConfirm;
    if (evt == ButtonEvent::LONG_PRESS)
    {
        _queue.push(SystemAction::END_SETUP);
    }
    else if (evt == ButtonEvent::CLICK)
    {
        _queue.push(waiting ? SystemAction::NONE : SystemAction::MOVE_NEXT);
    }
}

void ButtonManager::setup_UP(ButtonEvent evt)
{
    bool waiting = _view.setup.isWaitingConfirm;
    if (evt == ButtonEvent::CLICK)
    {
        _queue.push(waiting ? SystemAction::SAVE_YES : SystemAction::VALUE_UP);
    }
}

void ButtonManager::setup_DOWN(ButtonEvent evt)
{
    bool waiting = _view.setup.isWaitingConfirm;
    if (evt == ButtonEvent::CLICK)
    {
        _queue.push(waiting ? SystemAction::SAVE_NO : SystemAction::VALUE_DOWN);
    }
}

void ButtonManager::setup_ENTER(ButtonEvent evt)
{
    bool waiting = _view.setup.isWaitingConfirm;
    if (evt == ButtonEvent::CLICK)
    {
        _queue.push(waiting ? SystemAction::NONE : SystemAction::CONFIRM);
    }
}

void ButtonManager::setup_NOP(ButtonEvent evt) { (void)evt; }
