#include "Safety.h"

Safety::Safety(uint8_t relayPin, uint8_t buzzerPin)
{
    _relayPin = relayPin;
    _buzzerPin = buzzerPin;

    _state = SAFE;

    _relayState = false;
    _buzzerState = false;

    _relayTimer = 0;
    _buzzerTimer = 0;
}

void Safety::begin()
{
    pinMode(_relayPin, OUTPUT);
    pinMode(_buzzerPin, OUTPUT);

    digitalWrite(_relayPin, LOW);
    digitalWrite(_buzzerPin, LOW);
}

void Safety::update(Battery &battery)
{
    detectFaults(battery);

    relayControl();

    buzzerControl();
}

SafetyState Safety::getState()
{
    return _state;
}

void Safety::detectFaults(Battery &battery)
{
    float maxVoltage = battery.getCellVoltage(battery.getStrongestCell());
    float minVoltage = battery.getCellVoltage(battery.getWeakestCell());

    if (maxVoltage > 4.20 || minVoltage < 3.00)
    {
        _state = SHUTDOWN;
    }
    else if (battery.getImbalance() >= 0.20f)
    {
        _state = CRITICAL;
    }
    else if (battery.getImbalance() >= 0.10f)
    {
        _state = WARNING;
    }
    else
    {
        _state = SAFE;
    }
}

void Safety::relayControl()
{
    if (_state == SHUTDOWN)
        digitalWrite(_relayPin, HIGH);
    else
        digitalWrite(_relayPin, LOW);
}

void Safety::buzzerControl()
{
    switch (_state)
    {
        case SAFE:
            digitalWrite(_buzzerPin, LOW);
            break;

        case WARNING:
            if (millis() - _buzzerTimer >= 1000)
            {
                _buzzerTimer = millis();
                _buzzerState = !_buzzerState;
                digitalWrite(_buzzerPin, _buzzerState);
            }
            break;

        case CRITICAL:
            if (millis() - _buzzerTimer >= 300)
            {
                _buzzerTimer = millis();
                _buzzerState = !_buzzerState;
                digitalWrite(_buzzerPin, _buzzerState);
            }
            break;

        case SHUTDOWN:
            digitalWrite(_buzzerPin, HIGH);
            break;
    }
}