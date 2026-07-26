#include <Arduino.h>
#include "Telemetry.h"
#include "Battery.h"
#include "Safety.h"
#include "Display.h"
#include "Runtime.h"

const uint8_t batteryPins[MAX_CELLS] = {34, 35, 32, 33};

Battery battery(batteryPins);
Safety safety(18, 19);
Display display;
Telemetry telemetry;
Runtime runtime(battery,
                safety,
                display,
                telemetry);

                
void setup()
{
    Serial.begin(115200);

    runtime.begin();

    Serial.println("ESP32 Smart BMS Started");
}

void loop()
{
    runtime.update();
}