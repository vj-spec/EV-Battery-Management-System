#include "Display.h"

Display::Display() : lcd(0x27, 16, 2)
{
    _lastUpdate = 0;
    _screen = false;
}

void Display::begin()
{
    lcd.init();
    lcd.backlight();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Smart BMS");
    lcd.setCursor(0, 1);
    lcd.print("Initializing");
    delay(1500);

    lcd.clear();
}

void Display::update(Battery &battery, Safety &safety)
{
    if (millis() - _lastUpdate >= 2000)
    {
        _lastUpdate = millis();

        lcd.clear();

        if (_screen)
            showScreen1(battery);
        else
            showScreen2(battery, safety);

        _screen = !_screen;
    }
}

void Display::showScreen1(Battery &battery)
{
    lcd.setCursor(0, 0);
    lcd.print("PACK:");
    lcd.print(battery.getPackVoltage(), 2);
    lcd.print("V");

    lcd.setCursor(0, 1);
    lcd.print("SOC:");
    lcd.print(battery.getSOC(), 0);
    lcd.print("%");
}

void Display::showScreen2(Battery &battery, Safety &safety)
{
    lcd.setCursor(0, 0);
    lcd.print("SOH:");
    lcd.print(battery.getSOH(), 0);
    lcd.print("%");

    lcd.setCursor(0, 1);

    switch (safety.getState())
    {
        case SAFE:
            lcd.print("SAFE");
            break;

        case WARNING:
            lcd.print("WARNING");
            break;

        case CRITICAL:
            lcd.print("CRITICAL");
            break;

        case SHUTDOWN:
            lcd.print("SHUTDOWN");
            break;
    }
}