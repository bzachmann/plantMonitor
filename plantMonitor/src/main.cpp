#include <Arduino.h>

#include "wifimanager.h"
#include "mqttmanager.h"
#include "DHT.h"
#include "countdowntimer.h"
#include "soilmoisturesensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTTYPE DHT22
#define DHTPIN  13

#define ONE_WIRE_PIN  23


DHT dht(DHTPIN, DHTTYPE);
SoilMoistureSensor moistureSensor(34, 1740, 3420, 0.01f); //air: 3450, dry soil: 3420, cup water: 1615, wet soil: 1740

CountdownTimer printTimer;

DeviceAddress soilTempSensorAddress;
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature dallasSensors(&oneWire);


void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();
  moistureSensor.init();

  dallasSensors.begin();
  if(!dallasSensors.getAddress(soilTempSensorAddress, 0))
  {
    Serial.println("Unable to find soil temp sensor");
  }
  dallasSensors.setResolution(soilTempSensorAddress, 11);
  

  MqttManager::inst.init();

  printTimer.setDuration(500);
  printTimer.start();
}

void loop() {
  WifiManager::inst.update();

  float moisture = moistureSensor.read();
  MqttManager::inst.setSoilMoisture(moisture);

  printTimer.update();
  if(printTimer.isExpired())
  {
    printTimer.setDuration(500);
    dallasSensors.requestTemperatures();
    float soilTemp = dallasSensors.getTempF(soilTempSensorAddress);
    Serial.println(soilTemp);
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