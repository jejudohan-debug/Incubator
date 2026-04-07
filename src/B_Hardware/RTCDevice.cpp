#include "RTCDevice.h"

void RTCDevice::init()
{
    if (!_rtc.begin()) {
        Serial.println(F("RTC not Found!"));
    }

    // RTC 시간이 설정되지 않았을 경우, 컴파일된 시간으로 설정
    if (_rtc.lostPower()) {
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    Serial.println(F("Virtual RTC Started..."));
}

void RTCDevice::update()
{
    const unsigned long READ_INTERVAL = 1000;
    unsigned long current = millis();

    if (current - _lastReadTime < READ_INTERVAL)
        return;

    _lastReadTime = current;

    uint32_t currentUnix = getUnixTime();
    if (currentUnix == 0)
    {
        //Serial.print(F("RTC Error: Get time failed! Time="));
        //Serial.println(current);
        return;
    }
    notify(EventFlag::RTC_TIME, currentUnix);
}

uint32_t RTCDevice::getUnixTime()
{
    return _rtc.now().unixtime();
}

void RTCDevice::setTime(uint32_t timestamp)
{
    _rtc.adjust(DateTime(timestamp));
}
