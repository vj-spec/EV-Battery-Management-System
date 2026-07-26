#ifndef SAFETY_H
#define SAFETY_H

#include <Arduino.h>
#include "Battery.h"

enum SafetyState
{
    SAFE,
    WARNING,
    CRITICAL,
    SHUTDOWN
};

class Safety
{
public:
    Safety(uint8_t relayPin, uint8_t buzzerPin);

    void begin();

    void update(Battery &battery);

    SafetyState getState();

private:
    uint8_t _relayPin;
    uint8_t _buzzerPin;

    SafetyState _state;

    unsigned long _relayTimer;
    unsigned long _buzzerTimer;

    bool _relayState;
    bool _buzzerState;

    void detectFaults(Battery &battery);

    void relayControl();

    void buzzerControl();
};

#endif