#pragma once

#include <Arduino.h>
#include "A_Core/SystemContext.h"
#include "B_Hardware/HardwareFactory.h"

class ButtonManager : public IManager
{
private:
    Button *_btns = HardwareFactory::getButtons();

    DisplayState &_view = SystemContext::getView();
    EventQueue &_queue = SystemContext::getQueue();

    using NormalEventFunc = void (ButtonManager::*)(ButtonEvent evt);
    static const NormalEventFunc _normalFuncTable[];

    using SetupEventFunc = void (ButtonManager::*)(ButtonEvent evt);
    static const SetupEventFunc _setupFuncTable[];

    // 각 버튼별 Normal 모드 핸들러 (테이블로 매핑)
    void normal_SELECT(ButtonEvent evt);
    void normal_UP(ButtonEvent evt);
    void normal_DOWN(ButtonEvent evt);
    void normal_ENTER(ButtonEvent evt);
    void normal_NOP(ButtonEvent evt);

    // Setup 모드용 핸들러 선언
    void setup_SELECT(ButtonEvent evt);
    void setup_UP(ButtonEvent evt);
    void setup_DOWN(ButtonEvent evt);
    void setup_ENTER(ButtonEvent evt);
    void setup_NOP(ButtonEvent evt);

public:
    ButtonManager() = default;
    // 하드웨어 초기화 (setup에서 호출)
    void init();

    // 매 루프마다 호출
    void update();

    void handleEvent(BtnIdx idx, ButtonEvent evt);
    void handleNormalMode(BtnIdx idx, ButtonEvent evt);
    void handleSetupMode(BtnIdx idx, ButtonEvent evt);

    // 실시간 상태 확인 (필요 시)
    // bool isPressed(BtnIdx idx) const {
    //    return _btns[static_cast<int>(idx)].isPressed();
    //}
};