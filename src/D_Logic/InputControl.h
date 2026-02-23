#pragma once
#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"
#include "C_Manager/ManagerFactory.h"

class NormalControl;
class SetupControl;

class InputControl : public IControl
{
private:
    ButtonManager* _buttonManager;
    NormalControl* _normalCtrl;
    SetupControl* _setupCtrl;

    DisplayState &_view = SystemContext::getView();
    SystemConfig &_config = SystemContext::getConfig();
    EventQueue &_queue = SystemContext::getQueue();

public:
    InputControl();

    void init() override;
    void update() override;
    void handleAction(SystemAction action) override;

    DisplayState &getView() { return _view; }
    SystemConfig &getConfig() { return _config; }
};
