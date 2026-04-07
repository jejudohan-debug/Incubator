#include "DHTSensor.h"

// --- DHTSensor Methods ---
void DHTSensor::init()
{
    _dht.begin();

    _lastReadTime = millis();

    Serial.println(F("DHT22: Initialized."));
}

void DHTSensor::update()
{
    unsigned long current = millis();
    if (_lastReadTime != 0 && (current - _lastReadTime < DHT_READ_INTERVAL)) {
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

    _lastTemp = t;
    _lastHumi = h;

    notify(EventFlag::DHT_TEMP, t);
    notify(EventFlag::DHT_HUMI, h);
}

bool DHTSensor::isValid() const
{
    return !(isnan(_lastTemp) || isnan(_lastHumi));
}
