#include "ConfigRtcStorage.h"

void ConfigRtcStorage::init()
{
    memset(&_config, 0, sizeof(SystemConfig));

    if (!load())
    {
        resetSpeciesConfig();
    }
    exportViewConfigValue();
}

void ConfigRtcStorage::update()
{
    if (UpdateFlag::hasFlag(_view.updateFlags, UpdateFlag::CFG_LOAD))
    {
        load();
    }
    else if (UpdateFlag::hasFlag(_view.updateFlags, UpdateFlag::CFG_SAVE))
    {
        save();
    }
}

bool ConfigRtcStorage::load()
{
    SystemConfig tempCfg;

    _rtcDevice.loadConfig(tempCfg);

    if (tempCfg.signature != MAGIC_SIG)
        return false;
    if (tempCfg.checksum != calculateChecksum(tempCfg))
        return false;

    _config = tempCfg;
    notify(EventFlag::RTC_LOAD, _config);

    return true;
}

// 2. 데이터 저장 (직렬화)
void ConfigRtcStorage::save()
{
    _config.signature = MAGIC_SIG;
    _config.checksum = calculateChecksum(_config);

    _rtcDevice.saveConfig(_config);
    notify(EventFlag::RTC_SAVE, _config);
}

uint8_t ConfigRtcStorage::calculateChecksum(const SystemConfig &cfg)
{
    uint8_t *ptr = (uint8_t *)&cfg;
    uint8_t sum = 0;
    // 마지막 checksum 필드 전까지 루프
    for (size_t i = 0; i < sizeof(SystemConfig) - 1; i++)
    {
        sum ^= ptr[i];
    }
    return sum;
}

// 3. 특정 종 선택 시 설정값 동기화
void ConfigRtcStorage::resetSpeciesConfig(Species s)
{
    memset(&_config, 0, sizeof(SystemConfig));

    const SpeciesProfile profile =
        SystemContext::getInstance().getSpeciesContext().getProfile(s);

    _config.signature = MAGIC_SIG;
    _config.selectedSpecies = s;
    _config.targetTemp = profile.targetTemp;
    _config.targetHumi = profile.targetHumi;
    _config.turnInterval = profile.turnInterval;
    _config.incubationStartTime = _rtcDevice.getUnixTime();

    exportViewConfigValue();
    save(); // 변경 즉시 NVRAM 저장
}

void ConfigRtcStorage::resetSpeciesConfig()
{
    Species s = _config.selectedSpecies;
    resetSpeciesConfig(s);
}

bool ConfigRtcStorage::isViewChanged()
{
    if (_config.targetTemp != _view.getTargetTempFixed())
        return true;
    if (_config.targetHumi != _view.getTargetHumiFixed())
        return true;
    if (_config.turnInterval != _view.getTurnInterval())
        return true;
    if (_config.turnDuration != _view.getTurnDuration())
        return true;
    if (_config.incubationStartTime != _view.getStartUnixTime())
        return true;
    if (_config.selectedSpecies != _view.getSpecies())
        return true;

    return false;
}

void ConfigRtcStorage::importViewConfigValue()
{
    if (!isViewChanged())
        return;

    _config.targetTemp = _view.getTargetTempFixed();
    _config.targetHumi = _view.getTargetHumiFixed();
    _config.turnInterval = _view.getTurnInterval();
    _config.turnDuration = _view.getTurnDuration();
    _config.incubationStartTime = _view.getStartUnixTime();
    _config.selectedSpecies = _view.getSpecies();

    save();
}

void ConfigRtcStorage::exportViewConfigValue()
{
    _view.setTargetTemp(_config.targetTemp);
    _view.setTargetHumi(_config.targetHumi);
    _view.setTurnInterval(_config.turnInterval);
    _view.setTurnDuration(_config.turnDuration);
    _view.setStartUnixTime(_config.incubationStartTime);
    _view.setSpecies(_config.selectedSpecies);
}