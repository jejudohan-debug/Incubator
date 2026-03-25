#pragma once
#include <Arduino.h>
#include "CoreTypes.h"

class DisplayState
{
private:
    uint16_t _currentTemp = 0;
    uint16_t _currentHumi = 0;
    uint16_t _targetTemp = 0;
    uint16_t _targetHumi = 0;

    uint16_t _turnInterval = 120; // 분 단위
    uint16_t _turnDuration = 12;  // 초 단위
    PageStep _pageStep = PageStep::ENV;
    Species _species = Species::CHICKEN;
    uint32_t _currentUnixTime = 0; // 초 단위
    uint32_t _startUnixTime = 0;   // 초 단위
    uint8_t _d = 0, _h = 0, _m = 0;
    int16_t _PID_Kp = 200, _PID_Ki = 5, _PID_Kd = 20;    // PID 인수
    
    template <typename T>
    void updateField(T &field, T newValue, UpdateFlag::Type flag);

    template <typename T>
    void updateValue(
        void (DisplayState::*setter)(T),
        T newValue, UpdateFlag::Type flag)
    {
        (this->*setter)(newValue);
        updateFlags |= flag;
    }

public:
    UpdateFlag::Type updateFlags = UpdateFlag::NONE;

    DisplayState() = default;
    DisplayState(const DisplayState &) = delete;
    DisplayState &operator=(const DisplayState &) = delete;
    // 방지: DisplayState myView = SystemContext::getInstance().getView();

    inline uint16_t floatToFixed(float val) const
    {
        return (uint16_t)(val * 10.0f + 0.5f);
    }

    inline float fixedToFloat(uint16_t val) const
    {
        return (float)val / 10.0f;
    }

    void uint16ToString(char *buf, uint16_t value);

    void setCurrentTemp(float val);
    void setCurrentTemp(uint16_t val);
    float getCurrentTempFloat() const;
    uint16_t getCurrentTempFixed() const;

    void setCurrentHumi(float val);
    void setCurrentHumi(uint16_t val);
    float getCurrentHumiFloat() const;
    uint16_t getCurrentHumiFixed() const;

    void setTargetTemp(float val);
    void setTargetTemp(uint16_t val);
    float getTargetTempFloat() const;
    uint16_t getTargetTempFixed() const;

    void setTargetHumi(float val);
    void setTargetHumi(uint16_t val);
    float getTargetHumiFloat() const;
    uint16_t getTargetHumiFixed() const;

    void setTurnInterval(uint16_t val);
    uint16_t getTurnInterval() const;
    void setTurnDuration(uint16_t val);
    uint16_t getTurnDuration() const;

    void setPageStep(PageStep val);
    PageStep getPageStep() const;

    void setSpecies(Species s);
    Species getSpecies() const;
    // void importConfigValue(const SystemConfig &cfg);

    void setCurrentUnixTime(uint32_t val);
    uint32_t getCurrentUnixTime();
    void setStartUnixTime(uint32_t t);
    uint32_t getStartUnixTime() const;
    uint32_t getElapsedUnixTime() const;

    void calculateElapsedTime();
    uint8_t getElapsedDay() const;
    uint8_t getElapsedHour() const;
    uint8_t getElapsedMinute() const;

    void updateRelayFlag(UpdateFlag::Type flag);
    bool getRelayFlag(UpdateFlag::Type flag);

    void setPID_Kp(int16_t val);
    void setPID_Ki(int16_t val);
    void setPID_Kd(int16_t val);
    int16_t getPID_Kp() const;
    int16_t getPID_Ki() const;
    int16_t getPID_Kd() const;
    
    /*template <typename SetterFunc>
    void InputControl::updateActionState(
        SetterFunc setter, bool enable, ActionStateFlag::Type uFlag)
    {
        // _view 객체의 멤버 함수를 실행합니다.
        (_view.*setter)(enable);
        _view.updateFlags |= uFlag;
    }*/
    // 호출 시 사용법:
    // updateActionState(&View::setRelayHeat, true, UpdateFlag::R_HEAT);
    // updateActionState(&View::setRelayTurn, false, UpdateFlag::R_TURN);
};