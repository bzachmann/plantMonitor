#include <Arduino.h>

#include "wifimanager.h"
#include "mqttmanager.h"
#include "DHT.h"
#include "countdowntimer.h"
#include "soilmoisturesensor.h"

#define DHTTYPE DHT22
#define DHTPIN  13

DHT dht(DHTPIN, DHTTYPE);
SoilMoistureSensor moistureSensor(34, 1615, 3450, 0.01f);

CountdownTimer printTimer;


void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();
  moistureSensor.init();

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