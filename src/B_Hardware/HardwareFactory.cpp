#include "HardwareFactory.h"

HardwareFactory &HardwareFactory::getInstance()
{
    static HardwareFactory instance;
    return instance;
}

LcdDisplay &HardwareFactory::getDisplay()
{
    static LcdDisplay display;
    return display;
}

DHTSensor &HardwareFactory::getDHT()
{
    static DHTSensor instance(Pins::DHT);
    return instance;
}

RTCDevice &HardwareFactory::getRTC()
{
    static RTCDevice rtc(Pins::RTC_DAT, Pins::RTC_CLK, Pins::RTC_RST);
    return rtc;
}

Button::Group<3> &HardwareFactory::getButtons()
{
    static Button::Group<3> btns =
        {{Button(Pins::BTN_SELECT, EventFlag::BTN_SELECT),
          Button(Pins::BTN_UP, EventFlag::BTN_UP),
          Button(Pins::BTN_DOWN, EventFlag::BTN_DOWN)}};
    return btns;
}

RelayActuator::Group<3> &HardwareFactory::GetRelays()
{
    static RelayActuator::Group<3> relays =
        {{RelayActuator(Pins::HEATER),
          RelayActuator(Pins::FAN),
          RelayActuator(Pins::MOTOR)}};
    return relays;
}