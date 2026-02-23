#pragma once
#define DHT_TYPE DHT22

#include <DHT.h>

class DHTSensor
{
private:
    DHT _dht;
    uint8_t _pin;
    float _lastTemp = -99.0f;
    float _lastHumi = -99.0f;
    unsigned long _lastReadTime = 0;
    const unsigned long READ_INTERVAL = 3000; // 3 seconds for stability

public:
    DHTSensor(uint8_t pin) : _dht(pin, DHT_TYPE) {}
    ~DHTSensor() {};

    void init();
    void update();
    // 인터페이스 반환
    float getTemp() { return _lastTemp; }
    float getHumi() { return _lastHumi; }
};