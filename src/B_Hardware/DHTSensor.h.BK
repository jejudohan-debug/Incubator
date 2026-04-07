#pragma once
#define DHT_TYPE DHT22

#include <DHT.h>
#include "A_Core/Interfaces.h"

class DHTSensor : public IDevice
{
private:
    DHT _dht;

    float _lastTemp = -99.0f;
    float _lastHumi = -99.0f;
    unsigned long _lastReadTime = 0;
    uint8_t _errorCount = 0;

public:
    DHTSensor(uint8_t pin) : _dht(pin, DHT_TYPE) {}
    ~DHTSensor() {};

    void init() override;
    void update() override;
    bool isValid() const;

    // 인터페이스 반환
    float getTemp() { return _lastTemp; }
    float getHumi() { return _lastHumi; }
};