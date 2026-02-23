#pragma once
#include "A_Core/SystemContext.h"
#include "C_Manager/SensorManager.h"

class SensorControl : public IControl
{
private:
    SensorManager _sensor;

    DisplayState &_view = SystemContext::getView();
    SystemConfig &_config = SystemContext::getConfig();
    EventQueue &_queue = SystemContext::getQueue();

public:
    SensorControl();

    void init() override;
    void update() override;
    void handleAction(SystemAction action) override;

    void tempStrategy();
    void humiStrategy();
};