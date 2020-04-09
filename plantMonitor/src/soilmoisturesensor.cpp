#include "soilmoisturesensor.h"

#include <Arduino.h>

SoilMoistureSensor::SoilMoistureSensor(uint8_t pinVal, uint16_t wetCountsVal, uint16_t dryCountsVal, float gain):
    pin(pinVal),
    wetCounts(wetCountsVal),
    dryCounts(dryCountsVal),
    moisture(0.0f),
    filter(),
    timer()
{
    filter.setGain(gain);
}

void SoilMoistureSensor::init()
{
    filter.init(analogRead(pin));
    timer.setDuration(100);
    timer.start();
}

float SoilMoistureSensor::read()
{
    timer.update();
    if(timer.isExpired())
    {
        timer.setDuration(100);

        uint16_t counts = filter.filter(analogRead(34));
        counts = constrain(counts, wetCounts, dryCounts);
        uint16_t mappedCounts = map(counts, dryCounts, wetCounts, 0, 1000);
        moisture = static_cast<float>(mappedCounts) / 10;
    }

    return moisture;
}