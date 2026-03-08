#pragma once
#include "ControlFactory.h"
#include "InputControl.h"
#include "SensorControl.h"
#include "NormalControl.h"
#include "SetupControl.h"
#include "EnvControl.h"
#include "TurningControl.h"

class ControlFactory
{
public:
    ControlFactory() = default;

    ControlFactory(const ControlFactory&) = delete;
    ControlFactory& operator=(const ControlFactory&) = delete;

    ~ControlFactory() = default;

    static InputControl* getInputControl();
    static SensorControl* getSensorControl();
    static NormalControl* getNormalControl();
    static SetupControl* getSetupControl();
    static EnvControl* getEnvControl();
    static TurningControl* getTurningControl();
};