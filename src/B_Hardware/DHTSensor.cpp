#include "DHTSensor.h"

// --- DHTSensor Methods ---
void DHTSensor::init()
{
    _dht.begin();
    delay(2000); // Wait for sensor to stabilize
    update();
    /*if (isnan(_lastTemp) || _lastTemp == -99.0f) {
        Serial.println("DHT22: WARNING! First read failed (NaN). Check Wiring.");
    } else if (_lastTemp == 0.0f && _lastHumi == 0.0f) {
        Serial.println("DHT22: WARNING! Read 0.0/0.0. This is suspicious. Check Data Pin.");
    } else {
        Serial.print("DHT22: First read OK. T="); Serial.print(_lastTemp);
        Serial.print(", H="); Serial.println(_lastHumi);
    }*/
}

void DHTSensor::update()
{
    unsigned long current = millis();
    if (current - _lastReadTime >= READ_INTERVAL || _lastReadTime == 0)
    {
        _lastReadTime = current; // Update interval even if read fails to prevent spamming
        float t = _dht.readTemperature();
        float h = _dht.readHumidity();

        if (isnan(t) || isnan(h))
        {
            Serial.print(F("DHT22 Error: Read failed (NaN)! Time="));
            Serial.println(current);
        }
        else
        {
            _lastTemp = t;
            _lastHumi = h;
            if (t == 0.0f && h == 0.0f)
            {
                Serial.println(F("DHT22 Warning: Got EXACT 0.00/0.00."));
            }
        }
    }
}
