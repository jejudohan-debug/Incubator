#include "SystemContext.h"

SystemContext &SystemContext::getInstance()
{
    static SystemContext instance;
    return instance;
}

/*LiquidCrystal_I2C &SystemContext::getLcd()
{
    static LiquidCrystal_I2C lcd{0x27, 16, 2};
    static bool isInitialized = false;
    if (!isInitialized)
    {
        lcd.init();
        lcd.backlight();
        isInitialized = true;
    }
    return lcd;
}*/

DisplayState &SystemContext::getView()
{
    static DisplayState view;
    return view;
}

ActionQueue &SystemContext::getActionQueue()
{
    static ActionQueue queue;
    return queue;
}

/*SystemConfig &SystemContext::getConfig()
{
    static SystemConfig config;
    return config;
}*/

SpeciesContext &SystemContext::getSpeciesContext()
{
    static SpeciesContext speciesContext;
    return speciesContext;
}