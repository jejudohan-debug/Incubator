#pragma once

#include <Arduino.h>
#include <avr/wdt.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwareFactory.h"
#include "C_Manager/EventBtnManager.h"
#include "C_Manager/EventSSorManager.h"
#include "C_Manager/EventRtcManager.h"
#include "C_Manager/ConfigEEPROM.h"
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
    OutputManager _outManager;
    ConfigEEPROM _cfgEEPROM;
    InputControl _inputControl;
    RtcControl _rtcControl;
    SSorControl _ssorControl;

    IManager *_mgrs[3];

    LcdDisplay &_display = HardwareFactory::getInstance().getDisplay();
    DisplayState &_view = SystemContext::getInstance().getView();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

public:
    ActionLogic()
        : _evtBtnMgr(HardwareFactory::getInstance().getButtons()),
          _evtSSorMgr(HardwareFactory::getInstance().getDHT()),
          _evtRtcMgr(HardwareFactory::getInstance().getRTC()),
          _outManager(HardwareFactory::getInstance().GetRelays()),
          _inputControl(_cfgEEPROM),
          _rtcControl(_cfgEEPROM),
          _mgrs{&_evtBtnMgr, &_evtRtcMgr, &_evtSSorMgr} {}

    void init();
    void update();
};