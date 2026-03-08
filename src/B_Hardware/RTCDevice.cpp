#include "RTCDevice.h"

void RTCDevice::init()
{
    _rtc.Begin();

    // 시간 기록이 멈춰있다면(새 칩인 경우) 작동 시작
    if (!_rtc.GetIsRunning())
    {
        _rtc.SetIsRunning(true);
    }

    /*/ 컴파일 시간으로 초기화 (필요시)
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    if (_rtc.GetDateTime() < compiled) {
        _rtc.SetDateTime(compiled);
    }*/
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
        Serial.print(F("RTC Error: Get time failed! Time="));
        Serial.println(current);
        return;
    }
    notify(EventFlag::RTC_TIME, currentUnix);
}

uint32_t RTCDevice::getUnixTime()
{
    if (!_rtc.GetIsRunning())
        return 0;
    return _rtc.GetDateTime().Unix32Time();
}

void RTCDevice::setTime(uint32_t timestamp)
{
    _rtc.SetDateTime(RtcDateTime(timestamp));
}

void RTCDevice::saveConfig(const SystemConfig &config)
{
    // 구조체를 바이트 배열로 캐스팅하여 저장
    const uint8_t *data = reinterpret_cast<const uint8_t *>(&config);

    for (uint8_t i = 0; i < sizeof(SystemConfig); i++)
    {
        _rtc.SetMemory(i, data[i]);
    }
    notify(EventFlag::RTC_SAVE, config);
}

void RTCDevice::loadConfig(SystemConfig &config)
{
    uint8_t *data = reinterpret_cast<uint8_t *>(&config);

    for (uint8_t i = 0; i < sizeof(SystemConfig); i++)
    {
        data[i] = _rtc.GetMemory(i);
    }
    notify(EventFlag::RTC_LOAD, config);
}