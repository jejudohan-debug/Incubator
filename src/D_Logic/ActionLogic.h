#pragma once

#include <Arduino.h>
#include <avr/wdt.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwareFactory.h"
#include "C_Manager/EventBtnManager.h"
#include "C_Manager/EventSSorManager.h"
#include "C_Manager/EventRtcManager.h"
#include "C_Manager/ConfigRtcStorage.h"
#include "C_Manager/OutputManager.h"
#include "InputControl.h"
#include "SSorControl.h"
#include "RtcControl.h"

class ActionLogic
{
private:
    EventBtnManager _evtBtnMgr;
    EventSSorManager _evtSSorMgr;
    EventRtcManager _evtRtcMgr;
    ConfigRtcStorage _cfgRtcStor;
    OutputManager _outManager;
    InputControl _inputControl;
    SSorControl _ssorControl;
    RtcControl _rtcControl;

    IManager *_mgrs[3];

    LcdDisplay &_display = HardwareFactory::getInstance().getDisplay();
    DisplayState &_view = SystemContext::getInstance().getView();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

public:
    ActionLogic()
        : _evtBtnMgr(HardwareFactory::getInstance().getButtons()),
          _evtSSorMgr(HardwareFactory::getInstance().getDHT()),
          _evtRtcMgr(HardwareFactory::getInstance().getRTC()),
          _cfgRtcStor(HardwareFactory::getInstance().getRTC()),
          _outManager(HardwareFactory::getInstance().GetRelays()),
          _inputControl(_cfgRtcStor),
          _mgrs{&_evtBtnMgr, &_evtSSorMgr, &_evtRtcMgr} {}

    void init();
    void update();
};