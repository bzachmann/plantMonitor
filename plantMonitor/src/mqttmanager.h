#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <publishingmanager.h>

class MqttManager
{
public:
    MqttManager();

    void init();
    void update();

    void setWifiConnected(bool connected);

    void setTemperature(float temp);
    void setHumidity(float humidity);
    void setSoilMoisture(float moisture);
    void setSoilTemperature(float temp);

private:
    void manageMqttConnection();
    static void staticMqttCallback(char * topic, byte * data, unsigned int length);
    void mqttCallback(char * topic, byte * data, unsigned int length);
    void subscribeToTopics();

    static String statusToStr(int8_t status);
    static void printMqttPayload(char* topic, byte * data, unsigned int length);

public:
    static MqttManager inst;

private:
    bool wifiConnected;
    bool mqttConnected;
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    char const * clientID;

    PublishingManager temperaturePublisher;
    PublishingManager humidityPublisher;
    PublishingManager soilMoisturePublisher;
    PublishingManager soilTemperaturePublisher;
};

#endif /* MQTTMANAGER_H */