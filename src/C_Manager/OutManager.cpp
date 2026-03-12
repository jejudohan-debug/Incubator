#include "OutputManager.h"

void OutputManager::init()
{
    for (auto &relay : _relays)
    {
        relay.init();
    }
}

/*void OutputManager::switchingRelay(int index, UpdateFlag::Type flag, bool isOn)
{
    if (!UpdateFlag::hasFlag(_view.updateFlag, flag)) return;
}*/

void OutputManager::update()
{
    for (const auto &map : _relayMaps)
    {
        if (UpdateFlag::hasFlag(_view.updateFlags, UpdateFlag::RELAY_STATE))
        {
            bool isOn = (_operate.*(map.getter))();

            isOn ? _relays[map.index].run() : _relays[map.index].stop();
        }
    }
}
