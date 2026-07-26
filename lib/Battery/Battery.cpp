#include "Battery.h"

static float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Battery::Battery(const uint8_t pins[])
{
    for (int i = 0; i < MAX_CELLS; i++)
    {
        _pins[i] = pins[i];
        _raw[i] = 0;
        _cellVoltage[i] = 0;
    }

    _packVoltage = 0;
    _averageVoltage = 0;
    _imbalance = 0;

    _soc = 0;
    _soh = 100;

    _strongestCell = 0;
    _weakestCell = 0;

    _health = HEALTHY;
}

void Battery::begin()
{
    analogReadResolution(12);

    for (int i = 0; i < MAX_CELLS; i++)
    {
        pinMode(_pins[i], INPUT);
    }
}

void Battery::update()
{
    readCells();

    calculateVoltages();

    calculatePack();

    calculateSOC();

    calculateSOH();

    classifyHealth();
}

void Battery::readCells()
{
    for (int i = 0; i < MAX_CELLS; i++)
    {
        _raw[i] = analogRead(_pins[i]);
    }
}

void Battery::calculateVoltages()
{
    for (int i = 0; i < MAX_CELLS; i++)
    {
        float adcVoltage = (_raw[i] / 4095.0) * 3.3;

        // Simulated Li-Ion Cell (3.0V - 4.2V)
        _cellVoltage[i] = mapFloat(adcVoltage, 0.0, 3.3, 2.5, 4.3);
    }
}

void Battery::calculatePack()
{
    _packVoltage = 0;

    float minVoltage = 100;
    float maxVoltage = -100;

    for (int i = 0; i < MAX_CELLS; i++)
    {
        _packVoltage += _cellVoltage[i];

        if (_cellVoltage[i] < minVoltage)
        {
            minVoltage = _cellVoltage[i];
            _weakestCell = i;
        }

        if (_cellVoltage[i] > maxVoltage)
        {
            maxVoltage = _cellVoltage[i];
            _strongestCell = i;
        }
    }

    _averageVoltage = _packVoltage / MAX_CELLS;

    if (_averageVoltage > 0)
    {
        _imbalance = maxVoltage - minVoltage;
    }
    else
    {
        _imbalance = 0;
    }
}

void Battery::calculateSOC()
{
    float cellSOC = 0;

    for (int i = 0; i < MAX_CELLS; i++)
    {
        float soc = ((_cellVoltage[i] - 3.0) / 1.2) * 100.0;

        if (soc < 0) soc = 0;
        if (soc > 100) soc = 100;

        cellSOC += soc;
    }

    _soc = cellSOC / MAX_CELLS;
}

void Battery::calculateSOH()
{
    if (_imbalance < 0.05)
        _soh = 100;
    else if (_imbalance < 0.15)
        _soh = 95;
    else if (_imbalance < 0.30)
        _soh = 90;
    else
        _soh = 80;
}

void Battery::classifyHealth()
{
    for (int i = 0; i < MAX_CELLS; i++)
    {
        if (_cellVoltage[i] < 3.0f || _cellVoltage[i] > 4.2f)
        {
            _health = PACK_FAILURE;
            return;
        }
    }

if (_imbalance < 0.10f)
    _health = HEALTHY;
else if (_imbalance < 0.20f)
    _health = MINOR_IMBALANCE;
else
    _health = CRITICAL_IMBALANCE;
}

float Battery::getCellVoltage(uint8_t cell)
{
    if (cell >= MAX_CELLS)
        return 0;

    return _cellVoltage[cell];
}

float Battery::getPackVoltage()
{
    return _packVoltage;
}

float Battery::getAverageVoltage()
{
    return _averageVoltage;
}

float Battery::getImbalance()
{
    return _imbalance;
}

uint8_t Battery::getStrongestCell()
{
    return _strongestCell;
}

uint8_t Battery::getWeakestCell()
{
    return _weakestCell;
}

BatteryHealth Battery::getHealth()
{
    return _health;
}


float Battery::getSOC()
{
    return _soc;
}

float Battery::getSOH()
{
    return _soh;
}