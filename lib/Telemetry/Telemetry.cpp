#include "Telemetry.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

Telemetry::Telemetry()
{
    _lastUpdate = 0;
}

void Telemetry::begin()
{
    connectWiFi();
    connectBlynk();
}

void Telemetry::update(Battery &battery,
                       Safety &safety)
{
    Blynk.run();

    if (millis() - _lastUpdate >= 1000)
    {
        _lastUpdate = millis();

        sendData(battery, safety);
    }
}

void Telemetry::connectWiFi()
{
    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected");
}
void Telemetry::connectBlynk()
{
    Blynk.begin(BLYNK_AUTH_TOKEN,
                WIFI_SSID,
                WIFI_PASSWORD);

    Serial.println("Connected to Blynk");
}

void Telemetry::sendData(Battery &battery,
                         Safety &safety)
{
    Blynk.virtualWrite(V0, battery.getPackVoltage());
    Blynk.virtualWrite(V1, battery.getSOC());
    Blynk.virtualWrite(V2, battery.getSOH());
    Blynk.virtualWrite(V3, battery.getImbalance()); 

const char *healthText;

switch (battery.getHealth())
{
case HEALTHY:
    healthText = "Healthy";
    break;

case MINOR_IMBALANCE:
    healthText = "Minor Imbalance";
    break;

case CRITICAL_IMBALANCE:
    healthText = "Critical";
    break;

case PACK_FAILURE:
    healthText = "Pack Failure";
    break;

default:
    healthText = "Unknown";
}

Blynk.virtualWrite(V4, healthText);

const char *safetyText;

switch (safety.getState())
{
case SAFE:
    safetyText = "SAFE";
    break;

case WARNING:
    safetyText = "WARNING";
    break;

case CRITICAL:
    safetyText = "CRITICAL";
    break;

case SHUTDOWN:
    safetyText = "SHUTDOWN";
    break;

default:
    safetyText = "UNKNOWN";
}

Blynk.virtualWrite(V5, safetyText);
}