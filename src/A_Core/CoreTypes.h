#pragma once
#include <arduino.h>

enum class Species : uint8_t
{
    CHICKEN = 0,
    DUCK,
    GOOSE,
    QUAIL,
    BROODER,
    COUNT
};

// --- UI 상태 ---
enum class SystemModes : uint8_t
{
    NORMAL,
    SETUP,
    COUNT
};

enum class SystemAction : uint8_t
{
    NONE = 0,
    // --- Normal Mode Actions ---
    GO_SETUP,         // 설정 진입 (SELECT 1.5s)
    SHOW_PAGE_ENV,    // 환경 정보 (온습도)
    SHOW_PAGE_RELAYS, // 릴레이 상태
    SHOW_PAGE_CONFIG, //
    MOTOR_START,      // 모터 수동 시작
    MOTOR_STOP,       // 모터 수동 정지

    // --- Setup Mode Actions ---
    END_SETUP,  // 설정 종료 (SELECT 1.5s 또는 20S 입력 없음)
    MOVE_NEXT,  // 다음 단계 (SELECT Click)
    VALUE_UP,   // 수치 증가 (UP Click)
    VALUE_DOWN, // 수치 감소 (DOWN Click)
    CONFIRM,    // 확인
    SAVE_YES,   // 저장 확인: 예
    SAVE_NO,    // 저장 확인: 아니오
    BUTTON_LAST = SAVE_NO,
    // --- DHT ---
    TEMP_CHANGE,
    HUMI_CHANGE,
    SENSOR_LAST = HUMI_CHANGE,
    // --- RTC ---
    TIME_CHANGE,
    // ----------
    COUNT
};

enum class NormalPage : uint8_t
{
    ENV,
    RELAYS,
    CONFIG,
    COUNT
};

// 설정 모드 세부 단계
enum class SetupStep : uint8_t
{
    SPECIES = 0,
    DAY,
    HOUR,
    MINUTE,
    TEMPERATURE,
    HUMIDITY,
    TURNINTERVAL,
    COUNT
};

struct DeviceEvent
{
    SystemAction action; // 이제 버튼 정보가 아닌 '의도'를 담습니다.
};

// 버튼 인덱스를 위한 열거형 (가독성용)
enum class BtnIdx : uint8_t
{
    SELECT,
    UP,
    DOWN,
    ENTER,
    COUNT
};

enum class ButtonEvent : uint8_t
{
    NONE = 0, // 아무 일도 없음
    // --- 버튼 ---
    PRESS,        // 버튼이 눌렸음 (디바운스 후 안정된 상태)
    CLICK,        // 짧게 딸깍 누름 (메뉴 이동, 선택)
    LONG_PRESS,   // 2초 이상 꾹 누름 (설정 모드 진입, 리셋)
    DOUBLE_CLICK, // (선택 사항) 빠르게 두 번 누름
    ERROR_STATE,  // 버튼 회로 이상 등
};

enum class InternalState : uint8_t
{
    IDLE,       // 떼어진 상태
    DEBOUNCING, // 신호 변동 중 (안정화 대기)
    PRESSED,    // 눌린 상태 (안정화됨)
    LONG_HELD   // 이미 롱 프레스 이벤트를 발생시킨 상태
};

enum class DeviceType : uint8_t
{
    UNKNOWN = 0,
    // 액추에이터 계열
    HEATER,
    FAN,   // 히터 열 확산용 팬
    MOTOR, // 전란 모터
    // BUZZER,        // 알람용 부저
    //  입력 및 센서 계열
    DHT_SENSOR,
    TEMP_SENSOR,
    HUMIDITY_SENSOR,
    BUTTON,
    RTC_MODULE,
    RTC_TIME_PROVIDER,
    RTC_TIME_SETTER,
    RTC_CONFIG_STORAGE,
    LCD_DEVICE,
    LCD_DISPLAY
};

namespace Config
{
    // 안전 관련 설정
    namespace Safety
    {
        const int TEMP_MIN_SAFE = 0;
        const int TEMP_MAX_SAFE = 50;
        const int MAX_DEVICES = 10;
    }

    // 하드웨어 타이밍 설정
    namespace Timing
    {
        const unsigned long BUTTON_DEBOUNCE = 50;     // 50ms
        const unsigned long BUTTON_LONG_PRESS = 1500; // 1.5초
    }
};

struct SpeciesProfile
{
    const char *name;
    int totalDays;     // 총 부화 기간
    int hatchStartDay; // 파각 시작일
    float targetTemp;
    float targetHumi;
    float hatchHumi;  // 파각기 습기
    int turnInterval; // 분 단위 전란주기
};

struct NormalData
{
    NormalPage page = NormalPage::ENV;
    float currentTemp = -1.0;
    float currentHumi = -1.0;
    uint8_t relayBits = 0;

    bool operator==(const NormalData &other) const
    {
        return (page == other.page &&
                currentTemp == other.currentTemp &&
                currentHumi == other.currentHumi &&
                relayBits == other.relayBits);
    }
};

// 2. Setup 모드에서만 사용하는 데이터
struct SetupData
{
    SetupStep step = SetupStep::SPECIES;
    bool isWaitingConfirm = false;
    float targetTemp = -1.0;
    float targetHumi = -1.0;
    uint32_t turnInterval = 0;

    bool operator==(const SetupData &other) const
    {
        return (step == other.step &&
                isWaitingConfirm == other.isWaitingConfirm &&
                targetTemp == other.targetTemp &&
                targetHumi == other.targetHumi &&
                turnInterval == other.turnInterval);
    }
};

// 3. 통합 DisplayState
struct DisplayState
{
    bool isUpdated = true;
    SystemModes mode = SystemModes::NORMAL;
    Species species = Species::CHICKEN; // 기본값: CHICKEN
    uint8_t d = 0, h = 0, m = 0;

    // 모드별 데이터 객체 (구분하여 선언)
    NormalData normal;
    SetupData setup;

    void resetData()
    {
        d = 0;
        h = 0;
        m = 0;
        normal = NormalData(); // 기본값으로 초기화
        setup = SetupData();
    }

    // 모든 데이터를 한 번에 비교하기 위한 연산자 오버로딩 (선택 사항)
    bool operator==(const DisplayState &other) const
    {
        return (isUpdated == other.isUpdated &&
                mode == other.mode &&
                species == other.species &&
                d == other.d && h == other.h && m == other.m &&
                normal == other.normal &&
                setup == other.setup);
    }
};

struct __attribute__((packed)) SystemConfig
{
    uint16_t versionSignature;    // 2 bytes (예: 0xABCD)
    uint8_t selectedSpecies;      // 1 bytes
    float targetTemp;             // 4 bytes
    float targetHumid;            // 4 bytes
    uint32_t turnInterval;        // 4 bytes. 전란 주기 (초 단위)
    uint32_t incubationStartTime; // 4 bytes. 부화 시작 시각 (Unix epoch)
    // 총 19 바이트 (DS1302 31바이트 이내)

    bool operator==(const SystemConfig &other) const
    {
        return (versionSignature == other.versionSignature &&
                selectedSpecies == other.selectedSpecies &&
                targetTemp == other.targetTemp &&
                targetHumid == other.targetHumid &&
                turnInterval == other.turnInterval &&
                incubationStartTime == other.incubationStartTime);
    }

    // 세트로 != 도 만들어두면 편합니다
    bool operator!=(const SystemConfig &other) const
    {
        return !(*this == other);
    }
};

struct __attribute__((packed)) I2CSyncData
{
    float currentTemp;
    float currentHumi;
    uint32_t currentTime; // Slave의 현재 RTC 시각
    SystemConfig config;
};

struct __attribute__((packed)) I2CControlData
{
    uint8_t actuatorBits; // Bit 0: Heater, 1: Fan, 2: Motor
    SystemConfig config;  // Updated config from Master
};
