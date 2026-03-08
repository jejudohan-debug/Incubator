#include "DHTSensor.h"

// --- DHTSensor Methods ---
void DHTSensor::init()
{
    _dht.begin();

    _lastTemp = NAN;
    _lastHumi = NAN;
    _lastReadTime = millis();

    Serial.println(F("DHT22: Initialized. Waiting 2s for stabilization..."));
}

void DHTSensor::update()
{
    unsigned long current = millis();
    if (_lastReadTime != 0 && (current - _lastReadTime < READ_INTERVAL)) {
        return;
    }

    float t = _dht.readTemperature();
    float h = _dht.readHumidity();

    _lastReadTime = current;

    /*Serial.print(F("DHT Temperature : "));
    Serial.print(t);
    Serial.print(F(", DHT Humidity : "));
    Serial.println(h);*/

    if (isnan(t) || isnan(h))
    {
        _errorCount++;
        Serial.print(F("DHT22 Error: Read failed (NaN)! Time="));
        Serial.println(current);

        if (_errorCount >= 5) {
            Serial.println(F("Attempting to reset DHT sensor..."));
            _dht.begin();
            _errorCount = 0; // 카운트 초기화
        }
        return;
    }

    if (t == 0.0f && h == 0.0f)
    {
        Serial.println(F("DHT22 Warning: Got EXACT 0.00/0.00."));
    }

    updateAndNotify(EventFlag::DHT_TEMP, _lastTemp, t);
    updateAndNotify(EventFlag::DHT_HUMI, _lastHumi, h);
}

void DHTSensor::updateAndNotify(EventFlag::Type flag, float &lastValue, float newValue)
{
    if (abs(lastValue - newValue) >= 0.1f)
    {
        lastValue = newValue;
        notify(flag, newValue);
    }
}

bool DHTSensor::isValid() const
{
    return !(isnan(_lastTemp) || isnan(_lastHumi));
}
