#pragma once
#include <Arduino.h>
#include "A_Core/CoreTypes.h"
#include "A_Core/SystemContext.h"
#include "B_Hardware/RTCDevice.h"

class EventRtcManager : public IManager
{
private:
    static constexpr uint16_t MAGIC_SIG = 0xABCD;

    RTCDevice &_rtcDevice;
    SystemConfig _config;

    DisplayState &_view = SystemContext::getInstance().getView();
    ActionQueue &_actionQueue = SystemContext::getInstance().getActionQueue();

public:
    EventRtcManager(RTCDevice &rtc);

    void init();
    void update();

    void onNotify(EventFlag::Type flag, const uint32_t value) override;
    void onNotify(EventFlag::Type flag, const SystemConfig& value) override;

    bool isCurrect();
    void setSpecies(Species s);
    void setDefultConfig(Species defaultSpecies);
    const SystemConfig &getConfig() const { return _config; }
};
