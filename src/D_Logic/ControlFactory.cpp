#include "ControlFactory.h"

InputControl* ControlFactory::getInputControl() {
    static InputControl instance;
    return &instance;
}

SensorControl* ControlFactory::getSensorControl() {
    static SensorControl instance;
    return &instance;
}

NormalControl* ControlFactory::getNormalControl() {
    static NormalControl instance;
    return &instance;
}

SetupControl* ControlFactory::getSetupControl() {
    static SetupControl instance;
    return &instance;
}

EnvControl* ControlFactory::getEnvControl() {
    static EnvControl instance;
    return &instance;
}

TurningControl* ControlFactory::getTurningControl() {
    static TurningControl instance;
    return &instance;
}