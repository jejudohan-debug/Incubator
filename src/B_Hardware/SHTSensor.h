#pragma once

#include <Wire.h>
#include <adafruit_SHT31.h>
#include "A_Core/Interfaces.h"

class SHTSensor : public IDevice
{
private:
    uint8_t _address;
    Adafruit_SHT31 _sht30 = Adafruit_SHT31();

    float _lastTemp = -99.0f;
    float _lastHumi = -99.0f;
    unsigned long _lastReadTime = 0;
    uint8_t _errorCount = 0;
    bool _isHeaterEnabled = false;

public:
    SHTSensor(uint8_t address = 0x44) : _address(address) {}
    ~SHTSensor() {};

    void init() override;
    void update() override;
    bool isValid() const;

    void setHeater(bool active);
    bool isHeaterOn() const { return _isHeaterEnabled; }

    float getTemp() { return _lastTemp; }
    float getHumi() { return _lastHumi; }
};