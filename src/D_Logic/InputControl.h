#pragma once
#include "A_Core/Interfaces.h"
#include "A_Core/SystemContext.h"
#include "C_Manager/ConfigEEPROM.h"

#define ACTION_CNT 7
#define INCREASE_CNT 7

class InputControl
{
private:
    using ActionFunc = void (InputControl::*)();
    using IncreaseFunc = void (InputControl::*)();
    using DecreaseFunc = void (InputControl::*)();
    static const ActionFunc _actionMap[];
    static const IncreaseFunc _increaseMap[];
    static const DecreaseFunc _decreaseMap[];

    // ConfigRtcStorage &_cfgRtcStor;
    ConfigEEPROM &_cfgEEPROM;

    DisplayState &_view = SystemContext::getInstance().getView();
    OperateState &_operate = SystemContext::getInstance().getOperate();

public:
    InputControl(ConfigEEPROM &cfgEEPROM)
        : _cfgEEPROM(cfgEEPROM) {}

    void handleAction(SystemAction action);

private:
    void moveToNextStep();
    // void waitConfirm();
    void saveYes();
    void saveNo();
    // void autoTune();
    // void autoTuneYes();
    // void autoTuneNo();

    void increaseValue();
    void decreaseValue();

    void startHeat();
    void stopHeat();
    void startFan();
    void stopFan();
    void startTurn();
    void stopTurn();

    // Species
    void updateSelectedSpecies(Species newSpecies);
    void increaseSpecies();
    void decreaseSpecies();

    // 날짜
    void modifyStartTime(uint32_t seconds, bool bPlus);
    void increaseDay();
    void increaseHour();
    void increaseMinute();

    void decreaseDay();
    void decreaseHour();
    void decreaseMinute();

    void increaseTemperature();
    void increaseHumidity();
    void increaseTurnInterval();
    void increaseTurnDuration();

    void decreaseTemperature();
    void decreaseHumidity();
    void decreaseTurnInterval();
    void decreaseTurnDuration();

    void increasePID_Kp();
    void increasePID_Ki();
    void increasePID_Kd();
    void decreasePID_Kp();
    void decreasePID_Ki();
    void decreasePID_Kd();
};

// Relay On/Off
// void updateActionState(ActionStateFlag::Type flag,
//                      bool enable, ActionStateFlag::Type uFlag);

// 온도, 습도, 전란 주기
/*template <typename T>
void updateConfigValue(
    void (DisplayState::*setter)(T),
    T newValue, UpdateFlag::Type flag)
{
    (_view.*setter)(newValue);
    _view.updateFlags |= flag;
}*/
