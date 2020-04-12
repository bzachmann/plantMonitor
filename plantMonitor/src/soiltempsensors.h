#ifndef SOILTEMPSENSORS_H
#define SOILTEMPSENSORS_H

#include <Arduino.h>
#include "OneWire.h"
#include "DallasTemperature.h"
#include <countdowntimer.h>

class SoilTempSensors
{
public:
    SoilTempSensors(uint8_t pin);
    void init();
    void update();

    bool getTemp1(float & temp);

private:
    CountdownTimer updateTimer;
    OneWire oneWire;
    DallasTemperature dallasSensors;
    DeviceAddress temp1Address;

    bool temp1Connected;
    float temp1;
};


#endif /* SOILTEMPSENSORS_H */