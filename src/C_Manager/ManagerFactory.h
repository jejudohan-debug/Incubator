#pragma once

#include "LcdManager.h"
#include "ButtonManager.h"
#include "SensorManager.h"
#include "RtcManager.h"


class ManagerFactory
{
public:
    ManagerFactory() = default;

    ManagerFactory(const ManagerFactory&) = delete;
    ManagerFactory& operator=(const ManagerFactory&) = delete;

    ~ManagerFactory() = default;

    static LcdManager* getLcdManager();
    static ButtonManager* getButtonManager();
    static SensorManager* getSensorManager();
    static RtcManager* getRtcManager();
};