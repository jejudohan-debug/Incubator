#include "InputControl.h"
#include "NormalControl.h"
#include "SetupControl.h"
#include "ControlFactory.h"

InputControl::InputControl() {}

void InputControl::init()
{
    _buttonManager = ManagerFactory::getButtonManager();
    _normalCtrl = ControlFactory::getNormalControl();
    _setupCtrl = ControlFactory::getSetupControl();

    _buttonManager->init();
    _normalCtrl->init();
    _setupCtrl->init();
}

// 메인 갱신 루프
void InputControl::update()
{
    _buttonManager->update(); // 버튼 상태 갱신 (Debounce 등 처리)
}

void InputControl::handleAction(SystemAction action) {
    if (_view.mode == SystemModes::NORMAL)
    {
        _normalCtrl->handleAction(action);
    }
    else if (_view.mode == SystemModes::SETUP)
    {
        _setupCtrl->handleAction(action);
    }
}