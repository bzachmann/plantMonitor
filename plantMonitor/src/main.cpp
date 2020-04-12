#include <Arduino.h>

#include "wifimanager.h"
#include "mqttmanager.h"
#include "DHT.h"
#include "countdowntimer.h"
#include "soilmoisturesensor.h"
#include "soiltempsensors.h"

#define DHTTYPE DHT22
#define DHTPIN  13
#define ONE_WIRE_PIN  23


DHT dht(DHTPIN, DHTTYPE);
SoilMoistureSensor moistureSensor(34, 1740, 3420, 0.01f); //air: 3450, dry soil: 3420, cup water: 1615, wet soil: 1740
SoilTempSensors soilTempSensors(ONE_WIRE_PIN);

CountdownTimer printTimer;

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();
  moistureSensor.init();
  soilTempSensors.init();
  
  MqttManager::inst.init();

  printTimer.setDuration(500);
  printTimer.start();
}

void loop() {
  WifiManager::inst.update();

  float moisture = moistureSensor.read();
  MqttManager::inst.setSoilMoisture(moisture);

  soilTempSensors.update();
  float soilTemp1 = 0.0f;
  bool soilTemp1Valid = soilTempSensors.getTemp1(soilTemp1);

  printTimer.update();
  if(printTimer.isExpired())
  {
    printTimer.setDuration(500);
    
    Serial.print("Soil Temp: ");
    if(!soilTemp1Valid)
    {
      Serial.println("invalid");
    }
    else
    {
      Serial.println(soilTemp1);
    }
  }

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(true);
  if(!isnan(humidity) && !isnan(temperature))
  {
    MqttManager::inst.setTemperature(temperature);
    MqttManager::inst.setHumidity(humidity);
  }

  MqttManager::inst.setWifiConnected(WifiManager::inst.isConnected());
  MqttManager::inst.update();
}