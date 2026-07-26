#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

#define MAX_CELLS 4

enum BatteryHealth
{
    HEALTHY,
    MINOR_IMBALANCE,
    CRITICAL_IMBALANCE,
    PACK_FAILURE
};

class Battery
{
public:
    Battery(const uint8_t pins[]);

    void begin();
    void update();

    float getCellVoltage(uint8_t cell);
    float getPackVoltage();
    float getAverageVoltage();
    float getImbalance();

    uint8_t getStrongestCell();
    uint8_t getWeakestCell();

    BatteryHealth getHealth();

    float getSOC();
    float getSOH();

private:
    uint8_t _pins[MAX_CELLS];

    int _raw[MAX_CELLS];
    float _cellVoltage[MAX_CELLS];

    float _packVoltage;
    float _averageVoltage;
    float _imbalance;

    float _soc;
    float _soh;

    uint8_t _strongestCell;
    uint8_t _weakestCell;

    BatteryHealth _health;

    void readCells();
    void calculateVoltages();
    void calculatePack();
    void calculateSOC();
    void calculateSOH();
    void classifyHealth();

};

#endif