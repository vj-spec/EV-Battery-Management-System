#include "Runtime.h"

Runtime::Runtime(Battery &battery,
                 Safety &safety,
                 Display &display,
                 Telemetry &telemetry)
    : _battery(battery),
      _safety(safety),
      _display(display),
      _telemetry(telemetry)
{
    _initialized = false;
}

void Runtime::begin()
{
    _battery.begin();
    _safety.begin();
    _display.begin();
    _telemetry.begin();

    _initialized = true;
}

void Runtime::update()
{
    if (!_initialized)
        return;

    _battery.update();
    _safety.update(_battery);
    _display.update(_battery, _safety);
    _telemetry.update(_battery, _safety);

    static unsigned long lastPrint = 0;

    if (millis() - lastPrint >= 1000)
    {
        lastPrint = millis();

        Serial.println("\n========================================");
        Serial.println("        SMART BMS STATUS");
        Serial.println("========================================");

        Serial.print("Cell 1 Voltage : ");
        Serial.print(_battery.getCellVoltage(0));
        Serial.println(" V");

        Serial.print("Cell 2 Voltage : ");
        Serial.print(_battery.getCellVoltage(1));
        Serial.println(" V");

        Serial.print("Cell 3 Voltage : ");
        Serial.print(_battery.getCellVoltage(2));
        Serial.println(" V");

        Serial.print("Cell 4 Voltage : ");
        Serial.print(_battery.getCellVoltage(3));
        Serial.println(" V");

        Serial.print("Pack Voltage   : ");
        Serial.print(_battery.getPackVoltage());
        Serial.println(" V");

        Serial.print("Average Voltage: ");
        Serial.print(_battery.getAverageVoltage());
        Serial.println(" V");

        Serial.print("SOC            : ");
        Serial.print(_battery.getSOC());
        Serial.println(" %");

        Serial.print("SOH            : ");
        Serial.print(_battery.getSOH());
        Serial.println(" %");

        Serial.print("Imbalance      : ");
        Serial.print(_battery.getImbalance());
        Serial.println(" V");

        Serial.print("Strongest Cell : ");
        Serial.println(_battery.getStrongestCell() + 1);

        Serial.print("Weakest Cell   : ");
        Serial.println(_battery.getWeakestCell() + 1);

        Serial.print("Battery Health : ");

        switch (_battery.getHealth())
        {
        case HEALTHY:
            Serial.println("Healthy");
            break;

        case MINOR_IMBALANCE:
            Serial.println("Minor Imbalance");
            break;

        case CRITICAL_IMBALANCE:
            Serial.println("Critical Imbalance");
            break;

        case PACK_FAILURE:
            Serial.println("Pack Failure");
            break;
        }

        Serial.print("Safety State   : ");

        switch (_safety.getState())
        {
        case SAFE:
            Serial.println("SAFE");
            break;

        case WARNING:
            Serial.println("WARNING");
            break;

        case CRITICAL:
            Serial.println("CRITICAL");
            break;

        case SHUTDOWN:
            Serial.println("SHUTDOWN");
            break;
        }

        Serial.println("========================================");
    }

}