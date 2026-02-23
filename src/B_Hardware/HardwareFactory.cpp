#include "HardwareFactory.h"

LcdDisplay &HardwareFactory::getLcdDisplay()
{
    static LcdDisplay instance;
    return instance;
}

Button *HardwareFactory::getButtons()
{
    static Button btns[4] = {
        Button(Pins::BTN_SELECT),
        Button(Pins::BTN_UP),
        Button(Pins::BTN_DOWN),
        Button(Pins::BTN_ENTER)};
    return btns;
}

DHTSensor &HardwareFactory::getDHTSensor()
{
    static DHTSensor instance(Pins::DHT);
    return instance;
}

RTCDevice &HardwareFactory::getRTCDevice()
{
    static RTCDevice instance(Pins::RTC_DAT, Pins::RTC_CLK, Pins::RTC_RST);
    return instance;
}
