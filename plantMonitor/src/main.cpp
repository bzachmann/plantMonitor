#include <Arduino.h>

#include "wifimanager.h"
#include "mqttmanager.h"
#include "DHT.h"
#include "countdowntimer.h"
#include <filter.h>

#define DHTTYPE DHT22
#define DHTPIN  13

DHT dht(DHTPIN, DHTTYPE);
CountdownTimer printTimer;

Filter soilMoistureFilter;

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();

  soilMoistureFilter.setGain(0.01f);
  soilMoistureFilter.init(analogRead(34));

  MqttManager::inst.init();

  printTimer.setDuration(100);
  printTimer.start();
}

void loop() {
  WifiManager::inst.update();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(true);
  
  printTimer.update();
  if(printTimer.isExpired())
  {
    printTimer.setDuration(100);
    float ain = soilMoistureFilter.filter(analogRead(34));
    Serial.println(ain);
  }

  if(!isnan(humidity) && !isnan(temperature))
  {
    MqttManager::inst.setTemperature(temperature);
    MqttManager::inst.setHumidity(humidity);
  }

  MqttManager::inst.setWifiConnected(WifiManager::inst.isConnected());
  MqttManager::inst.update();
}