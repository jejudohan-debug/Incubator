#pragma once
#include <Arduino.h>

#define BUTTON_CNT 4
#define RELAY_CNT 2
#define TURN_DURATION 12          // SECONDS
#define DHT_READ_INTERVAL 2000    // 3 seconds for stability
#define SSR_CONTROL_INTERVAL 1000 // 1 seconds

template <typename T, size_t N>
struct NormalGroup
{
    T data[N];

    constexpr size_t size() const { return N; }
    T &operator[](size_t i) { return data[i]; }

    // 반복문을 위한 begin/end (포인터 반환)
    T *begin() { return &data[0]; }
    T *end() { return &data[N]; }
    const T *begin() const { return &data[0]; }
    const T *end() const { return &data[N]; }
};

enum class Species : uint8_t
{
    CHICKEN = 0,
    DUCK,
    GOOSE,
    QUAIL,
    BROODER,
    COUNT
};

enum class SystemAction : uint8_t
{
    NONE = 0,
    // --- Normal Mode Actions ---
    // GO_SETUP,         // 설정 진입 (SELECT 1.5s)
    // SHOW_PAGE_ENV, // 환경 정보 (온습도)
    // SHOW_PAGE_RELAYS, // 릴레이 상태
    // SHOW_PAGE_CONFIG, //
    MOVE_NEXT, // 1: 다음 단계 (SELECT Click)
    BUTTON_FIRST = MOVE_NEXT,
    HEATER_START, // 2: 모터 수동 시작
    HEATER_STOP,  // 3: 모터 수동 정지
    FAN_START,
    FAN_STOP,
    TURN_START,
    TURN_STOP,
    VALUE_UP,   // 8: 수치 증가 (UP Click)
    VALUE_DOWN, // 9: 수치 감소 (DOWN Click)
    // CONFIRM,    // 확인
    SAVE_YES, // 10: 저장 확인: 예
    SAVE_NO,  // 11: 저장 확인: 아니오
    AUTOTUNE,
    AUTOTUNE_YES,
    AUTOTUNE_NO,
    BUTTON_LAST = AUTOTUNE_NO,
    // --- DHT ---
    TEMP_CHANGE,
    HUMI_CHANGE,
    SENSOR_LAST = HUMI_CHANGE,
    // --- RTC ---
    TIME_CHANGE,
    LIMIT_SW,
    CFG_LOAD,
    CFG_SAVE,
    // ----------
    COUNT
};
constexpr int operator-(SystemAction a, SystemAction b)
{
    return static_cast<int>(a) - static_cast<int>(b);
}

enum class PageStep : uint8_t
{
    ENV = 0,
    CONFIG,
    // AUTOTUNING,
    SPECIES,
    SETUP_FIRST = SPECIES,
    DAY,
    HOUR,
    MINUTE,
    TARGET_TEMP,
    TARGET_HUMI,
    TURNINTERVAL,
    TURNDURATION,
    PID_KP,
    PID_KI,
    PID_KD,
    COUNT,
    SETUP_COUNT = COUNT - SETUP_FIRST
};
constexpr int operator-(PageStep a, PageStep b)
{
    return static_cast<int>(a) - static_cast<int>(b);
}

struct SpeciesProfile
{
    const char *name;
    uint8_t totalDays;     // 총 부화 기간
    uint8_t hatchStartDay; // 파각 시작일
    uint16_t targetTemp;
    uint16_t targetHumi;
    uint16_t hatchHumi;    // 파각기 습기
    uint16_t turnInterval; // 분 단위 전란주기

    float getTargetTemp() const { return targetTemp / 10.0f; }
    float getTargetHumi() const { return targetHumi / 10.0f; }
    float getHatchHumi() const { return hatchHumi / 10.0f; }
};

namespace EventFlag
{
    using Type = uint8_t;

    const Type NONE = 0;
    const Type BTN_SELECT = 1 << 0;
    const Type BTN_UP = 1 << 1;
    const Type BTN_DOWN = 1 << 2;
    const Type LIMIT_SW = 1 << 3;
    const Type DHT_TEMP = 1 << 4;
    const Type DHT_HUMI = 1 << 5;
    const Type RTC_TIME = 1 << 6;
    // const Type RTC_LOAD = 1 << 7;

    // const Type RTC_SAVE = 1 << 8;
    // const Type RELAY_ON_DONE = 1 << 9;
    // const Type RELAY_OFF_DONE = 1 << 10;
    // const Type RELAY_ERROR = 1 << 11;
    const Type ALL = 0xFF;

    inline bool hasFlag(Type source, Type target)
    {
        return (source & target) != 0;
    }
}

// 버튼용 옵저버 onNotify event
// void onNotify(EventFlag::Type flag, const uint8_t value)
enum class ButtonEvent : uint8_t
{
    NONE = 0,
    PRESS,
    CLICK,
    LONG_PRESS,
    RELEASE
    //    TEMP_CHANGE,
    //   HUMI_CHANGE,
    //    TIME_CHANGE
};

namespace OperateStateFlag
{
    using Type = uint8_t;

    const Type NONE = 0;
    const Type HEAT = 1 << 0;
    const Type FAN = 1 << 1;
    const Type TURN = 1 << 2;

    // 수동 모드 플래그들
    const Type M_HEAT = 1 << 3; // 히터 수동 제어 중
    const Type M_FAN = 1 << 4;  // 팬 수동 제어 중
    const Type M_TURN = 1 << 5; // 전란 수동 제어 중

    const Type WAITING = 1 << 6;
    const Type ALERT = 1 << 7;
    // const Type AUTOTUNE = 1 << 8;
    // const Type AUTOTUNEWAIT = 1 << 9;
    const Type ALL = 0xFF;

    inline bool hasFlag(Type source, Type target)
    {
        return (source & target) == target;
    }
}

namespace UpdateFlag
{
    using Type = uint16_t;

    const Type NONE = 0;
    const Type STEP = 1 << 0;
    const Type CURRENT_VALUE = 1 << 1;
    const Type TARGET_VALUE = 1 << 2;

    const Type RELAY_FAN = 1 << 3;  // 릴레이 상태 변경
    const Type RELAY_TURN = 1 << 4; // 개별로 on/off 반영
    const Type RELAY_HEAT = 1 << 5; // SSR

    const Type TIME = 1 << 6;
    const Type SPECIES = 1 << 7;
    // 6. 설정 데이터 처리 (EEPROM Load/Save)
    const Type CONFIG_EVENT = 1 << 8;
    const Type PID_GAIN = 1 << 9;
    // const Type AUTOTUNE = 1 << 10;

    const Type ALL = 0xFFFF; // 16비트 전체 갱신

    inline bool hasFlag(Type source, Type target)
    {
        return (source & target) != 0;
    }
}

struct __attribute__((packed)) SystemConfig
{
    uint16_t signature;      // 0xABCE (2 bytes)
    Species selectedSpecies; // Species enum (1 byte)
    uint16_t targetTemp;     // (2 bytes)
    uint16_t targetHumi;     // (2 bytes)
    uint16_t turnInterval;   // (2 bytes)
    uint16_t turnDuration;
    uint32_t incubationStartTime; // (4 bytes)
    // PID 게인 (10배 스케일링 된 정수)
    int16_t pidKp, pidKi, pidKd;

    uint8_t checksum; // 데이터 무결성 검사 (1 byte)

    bool operator==(const SystemConfig &other) const
    {
        return (signature == other.signature &&
                selectedSpecies == other.selectedSpecies &&
                targetTemp == other.targetTemp &&
                targetHumi == other.targetHumi &&
                turnInterval == other.turnInterval &&
                incubationStartTime == other.incubationStartTime);
    }

    // 세트로 != 도 만들어두면 편합니다
    bool operator!=(const SystemConfig &other) const
    {
        return !(*this == other);
    }
};
static_assert(sizeof(SystemConfig) <= 31, "SystemConfig is too large for DS1302 NVRAM!");
