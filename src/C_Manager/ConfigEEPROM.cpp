#include <EEPROM.h>
#include "ConfigEEPROM.h"

uint8_t ConfigEEPROM::calculateChecksum(const SystemConfig &cfg)
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

void ConfigEEPROM::init()
{
    memset(&_config, 0, sizeof(SystemConfig));

    if (!load())
    {
        resetSpeciesConfig();
    }
    exportViewConfigValue();
    if(isViewChanged())
    {
        Serial.println(F("ConfigEEPROM::init()-save() 실행"));
        importViewConfigValue();
        save();
    }
}

void ConfigEEPROM::update() {}

bool ConfigEEPROM::load()
{
    SystemConfig tempCfg;
    EEPROM.get(CONFIG_ADDR, tempCfg);

    if (tempCfg.signature != MAGIC_SIG)
        return false;
    if (tempCfg.checksum != calculateChecksum(tempCfg))
        return false;

    _config = tempCfg;

    return true;
}

// 2. 데이터 저장 (직렬화)
void ConfigEEPROM::save()
{
    Serial.println(F("ConfigEEPROM::save() 실행"));
    _config.signature = MAGIC_SIG;
    _config.checksum = calculateChecksum(_config);

    EEPROM.put(CONFIG_ADDR, _config);
}

// 3. 특정 종 선택 시 설정값 동기화
void ConfigEEPROM::resetSpeciesConfig(Species s)
{
    //memset(&_config, 0, sizeof(SystemConfig));

    const SpeciesProfile profile =
        SystemContext::getInstance().getSpeciesContext().getProfile(s);

    _config.signature = MAGIC_SIG;
    _config.selectedSpecies = s;
    _config.targetTemp = profile.targetTemp;
    _config.targetHumi = profile.targetHumi;
    _config.turnInterval = profile.turnInterval;
    _config.incubationStartTime = _view.getCurrentUnixTime();


    exportViewConfigValue();
    save(); // 변경 즉시 NVRAM 저장
}

void ConfigEEPROM::resetSpeciesConfig()
{
    Species s = _view.getSpecies();
    resetSpeciesConfig(s);
}

bool ConfigEEPROM::isViewChanged()
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
    if (_config.pidKp != _view.getPID_Kp()) {
        return true;
    }
    if (_config.pidKi != _view.getPID_Ki()) {
        return true;
    }
    if (_config.pidKd != _view.getPID_Kd())
        return true;

    return false;
}

void ConfigEEPROM::importViewConfigValue()
{
    if (!isViewChanged())
        return;

    _config.targetTemp = _view.getTargetTempFixed();
    _config.targetHumi = _view.getTargetHumiFixed();
    _config.turnInterval = _view.getTurnInterval();
    _config.turnDuration = _view.getTurnDuration();
    _config.incubationStartTime = _view.getStartUnixTime();
    _config.selectedSpecies = _view.getSpecies();
    _config.pidKp = _view.getPID_Kp();
    _config.pidKi = _view.getPID_Ki();
    _config.pidKd = _view.getPID_Kd();

    save();
}

void ConfigEEPROM::exportViewConfigValue()
{
    _view.setTargetTempFixed(_config.targetTemp);
    _view.setTargetHumiFixed(_config.targetHumi);
    _view.setTurnInterval(_config.turnInterval);
    _view.setTurnDuration(_config.turnDuration);
    _view.setStartUnixTime(_config.incubationStartTime);
    _view.setSpecies(_config.selectedSpecies);
    _view.setPID_Kp(_config.pidKp);
    _view.setPID_Ki(_config.pidKi);
    _view.setPID_Kd(_config.pidKd);
    _view.setPID_Ki(_config.pidKi);
    _view.setPID_Kd(_config.pidKd);
}