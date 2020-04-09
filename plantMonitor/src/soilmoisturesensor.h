#ifndef SOILMOISTURESENSOR_H
#define SOILMOISTURESENSOR_H

#include <Arduino.h>
#include <filter.h>
#include <countdowntimer.h>

class SoilMoistureSensor
{
public:
    SoilMoistureSensor(uint8_t pinVal, uint16_t wetCountsVal, uint16_t dryCountsVal, float gain);
    void init();
    float read();

private:
    uint8_t pin;
    uint16_t wetCounts;
    uint16_t dryCounts;
    float moisture;

    Filter filter;
    CountdownTimer timer;
};


#endif /* SOILMOISTURESENSOR_H */