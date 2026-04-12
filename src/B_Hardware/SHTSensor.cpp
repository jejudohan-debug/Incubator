#include "SHTSensor.h"

void SHTSensor::init()
{
    if (!_sht30.begin(_address))
    {
        Serial.println(F("SHT30: Failed to initialize."));
    }

    setHeater(false);

    _lastReadTime = millis();
    Serial.println(F("SHT30: Initialized."));
}

void SHTSensor::setHeater(bool active)
{
    _isHeaterEnabled = active;
    _sht30.heater(active);

    Serial.print(F("SHT30: Heater is now "));
    Serial.println(active ? F("ON") : F("OFF"));
}

void SHTSensor::update()
{
    unsigned long current = millis();
    if (_lastReadTime != 0 && (current - _lastReadTime < SHT_READ_INTERVAL))
    {
        return;
    }

    float t = _sht30.readTemperature();
    float h = _sht30.readHumidity();

    _lastReadTime = current;

    if (isnan(t) || isnan(h))
    {
        _errorCount++;
        Serial.print(F("SHT30 Error: Read failed (NaN)! Time="));
        Serial.println(current);

        if (_errorCount >= 5)
        {
            Serial.println(F("Attempting to reset SHT30 sensor..."));
            _sht30.begin(_address);
            _errorCount = 0; // 카운트 초기화
        }
        // return;
    }

    if (t == 0.0f && h == 0.0f)
    {
        Serial.println(F("SHT30 Warning: Got EXACT 0.00/0.00."));
    }

    if (isHeaterOn())
    {
        Serial.println(F("SHT30 Warning: Heater is ON, readings may be inaccurate."));
        // return;
    }

    _lastTemp = t;
    _lastHumi = h;
    if (t < 0) t = 0;
    if (h < 0) h = 0;
    notify(EventFlag::SHT_TEMP, (static_cast<uint16_t>(t * 100 + 0.5f)));
    notify(EventFlag::SHT_HUMI, (static_cast<uint16_t>(h * 100 + 0.5f)));
}

bool SHTSensor::isValid() const
{
    return !(isnan(_lastTemp) || isnan(_lastHumi));
}
