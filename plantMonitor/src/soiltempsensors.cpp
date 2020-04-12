#include "soiltempsensors.h"

SoilTempSensors::SoilTempSensors(uint8_t pin):
    updateTimer(),
    oneWire(pin),
    dallasSensors(&oneWire),
    temp1Address(),
    temp1Connected(false),
    temp1(0.0f)
{

}

void SoilTempSensors::init()
{
    dallasSensors.begin();
    updateTimer.setDuration(500);
    updateTimer.start();
}

void SoilTempSensors::update()
{   
    updateTimer.update();
    if(updateTimer.isExpired())
    {
        updateTimer.setDuration(500);

        if(!temp1Connected)
        {
            if(dallasSensors.getAddress(temp1Address, 0))
            {
                dallasSensors.setResolution(temp1Address, 11);
            }
        }

        temp1Connected = dallasSensors.requestTemperaturesByAddress(temp1Address);

        if(temp1Connected)
        {
            temp1 = dallasSensors.getTempF(temp1Address);  
        }
    }    
}

bool SoilTempSensors::getTemp1(float & temp)
{
    if(temp1Connected)
    {
        temp = temp1;
    }
    else
    {
        temp = 0.0f;
    }

    return temp1Connected;
    
}