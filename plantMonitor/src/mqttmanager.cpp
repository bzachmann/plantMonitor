#include "mqttmanager.h"

#include "privateinfo.h"


MqttManager MqttManager::inst;

MqttManager::MqttManager():
  wifiConnected(false),
  mqttConnected(false),
  wifiClient(),
  mqttClient(wifiClient),
  clientID("WateringSystemPlantMonitor")
{

}

void MqttManager::init()
{
  mqttClient.setServer(mqtt_server_addr, mqtt_server_port);
  mqttClient.setCallback(staticMqttCallback);
}

void MqttManager::update()
{
  manageMqttConnection();

  mqttClient.loop();

}

void MqttManager::setWifiConnected(bool connected)
{
  wifiConnected = connected;
}

void MqttManager::manageMqttConnection() 
//Should only be called if connected to network
//returns true if successfully connected to mqtt server
{
  mqttConnected = false;
  
  if(wifiConnected)
  {
    static bool subscribed = false;

    if(mqttClient.state() != MQTT_CONNECTED)
    {
      subscribed = false;

      static uint32_t time = millis() + 6000;
      uint32_t newTime = millis();

      if(   (time > newTime) //rollover
         || ((newTime - time) > 5000))
      {
        time = newTime;
        Serial.println("");
        Serial.println("Connecting mqtt");
        mqttClient.disconnect();
        mqttClient.connect(clientID);
      }

      static int8_t state = 50;
      int8_t newState = mqttClient.state();

      if(newState != state)
      {
        state = newState;
        Serial.print("MQTT status: ");
        Serial.println(statusToStr(newState));
      }
    }
    else
    {    
      mqttConnected = true;

      if(!subscribed)
      {
        subscribed = true;

        Serial.println("");
        Serial.println("MQTT connected");
        Serial.println("Subscribing to topics");
        subscribeToTopics();
      }
    }
  }
} 

void MqttManager::staticMqttCallback(char * topic, byte * data, unsigned int length)
{
  inst.mqttCallback(topic, data, length);
}

void MqttManager::mqttCallback(char * topic, byte * data, unsigned int length)
{
  printMqttPayload(topic, data, length);
}

void MqttManager::subscribeToTopics()
{

}

String MqttManager::statusToStr(int8_t status)
{
    String retVal = "";

    switch(status)
    {
        case static_cast<int8_t>(MQTT_CONNECTION_TIMEOUT):
        retVal = "MQTT_CONNECTION_TIMEOUT";
        break;

        case static_cast<int8_t>(MQTT_CONNECTION_LOST):
        retVal = "MQTT_CONNECTION_LOST";
        break;

        case static_cast<int8_t>(MQTT_CONNECT_FAILED):
        retVal = "MQTT_CONNECT_FAILED";
        break;

        case static_cast<int8_t>(MQTT_DISCONNECTED):
        retVal = "MQTT_DISCONNECTED";
        break;

        case static_cast<int8_t>(MQTT_CONNECTED):
        retVal = "MQTT_CONNECTED";
        break;

        case static_cast<int8_t>(MQTT_CONNECT_BAD_PROTOCOL):
        retVal = "MQTT_CONNECT_BAD_PROTOCOL";
        break;

        case static_cast<int8_t>(MQTT_CONNECT_BAD_CLIENT_ID):
        retVal = "MQTT_CONNECT_BAD_CLIENT_ID";
        break;

        case static_cast<int8_t>(MQTT_CONNECT_UNAVAILABLE):
        retVal = "MQTT_CONNECT_UNAVAILABLE";
        break;

        case static_cast<int8_t>(MQTT_CONNECT_BAD_CREDENTIALS):
        retVal = "MQTT_CONNECT_BAD_CREDENTIALS";
        break;

        case static_cast<int8_t>(MQTT_CONNECT_UNAUTHORIZED):
        retVal = "MQTT_CONNECT_UNAUTHORIZED";
        break;

        default:
        retVal = "UKNOWN";
        break;
    }

    return retVal;
}

void MqttManager::printMqttPayload(char* topic, byte * data, unsigned int length)
{
  Serial.println("");
  Serial.print("New MQTT message from topic ");
  Serial.print(topic);
  Serial.print(": ");
  for(uint16_t i = 0; i < length; i++)
  {
      Serial.print((char)data[i]);
  }
  Serial.println("");
}

