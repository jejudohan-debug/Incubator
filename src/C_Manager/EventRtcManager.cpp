#include "EventRtcManager.h"

// EventRtcManager::EventRtcManager(uint8_t dat, uint8_t clk, uint8_t rst)
//     : _rtcDevice(dat, clk, rst) {}
EventRtcManager::EventRtcManager(RTCDevice &rtc)
    : _rtcDevice(rtc) {}

void EventRtcManager::init()
{
    _rtcDevice.init();
    _rtcDevice.addObserver(this);
}

void EventRtcManager::update()
{
    _rtcDevice.update();
}

// RTC Minute Changed
void EventRtcManager::onNotify(EventFlag::Type flag, const uint32_t value)
{
    if (flag != EventFlag::RTC_TIME)
        return;

    _view.setCurrentUnixTime(value);
    _actionQueue.push(SystemAction::TIME_CHANGE);
}

/*void EventRtcManager::onNotify(EventFlag::Type flag, const SystemConfig& value)
{
    if (flag == EventFlag::RTC_LOAD) {
        _actionQueue.push(SystemAction::CFG_LOAD);
    } else if (flag == EventFlag::RTC_SAVE) {
        _actionQueue.push(SystemAction::CFG_SAVE);
    }
}*/
