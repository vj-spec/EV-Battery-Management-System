#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "Config.h"

#include "Battery.h"
#include "Safety.h"


class Telemetry
{
public:
    Telemetry();

    void begin();

    void update(Battery &battery,
                Safety &safety);

private:
    unsigned long _lastUpdate;

    void connectWiFi();

    void connectBlynk();

    void sendData(Battery &battery,
                  Safety &safety);
};

#endif