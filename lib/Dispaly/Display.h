#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Battery.h"
#include "Safety.h"

class Display
{
public:
    Display();

    void begin();

    void update(Battery &battery, Safety &safety);

private:
    LiquidCrystal_I2C lcd;

    unsigned long _lastUpdate;
    bool _screen;

    void showScreen1(Battery &battery);

    void showScreen2(Battery &battery, Safety &safety);
};

#endif