#ifndef RUNTIME_H
#define RUNTIME_H
#include "Telemetry.h"
#include <Arduino.h>
#include "Battery.h"
#include "Safety.h"
#include "Display.h"

class Runtime
{
public:
    Runtime(Battery &battery,
        Safety &safety,
        Display &display,
        Telemetry &telemetry);

    void begin();

    void update();

private:
    Battery &_battery;
    Safety &_safety;
    Display &_display;
    Telemetry &_telemetry;

    bool _initialized;
};

#endif