#include <Arduino.h>

#include "wifimanager.h"
#include "mqttmanager.h"


void setup() {
  Serial.begin(115200);
  delay(10);

  MqttManager::inst.init();  
}

void loop() {
  WifiManager::inst.update();

  MqttManager::inst.setWifiConnected(WifiManager::inst.isConnected());
  MqttManager::inst.update();
}