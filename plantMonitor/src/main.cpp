#include <Arduino.h>

#include "wifimanager.h"
#include "mqttmanager.h"
#include "DHT.h"
#include "countdowntimer.h"

#define DHTTYPE DHT22
#define DHTPIN  13

DHT dht(DHTPIN, DHTTYPE);
CountdownTimer printTimer;

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();

  MqttManager::inst.init();  

  printTimer.setDuration(1000);
  printTimer.start();
}

void loop() {
  WifiManager::inst.update();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(true);

  bool dhtValid = true;
  if(isnan(humidity) || isnan(temperature))
  {
    dhtValid = false;
  }
  
  printTimer.update();
  if(printTimer.isExpired())
  {
    printTimer.setDuration(1000);
    if(!dhtValid)
    {
      Serial.println("dht invalid");
    }

    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Temperature: ");
    Serial.println(temperature);
  }

  MqttManager::inst.setWifiConnected(WifiManager::inst.isConnected());
  MqttManager::inst.update();
}