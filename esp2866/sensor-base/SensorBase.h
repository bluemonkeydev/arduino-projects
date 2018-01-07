#ifndef SensorBase_h
#define SensorBase_h

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

// Support for real-time clock
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <PubSubClient.h>
#include <Metro.h>

#define MAX_SUBSCRIPTIONS 25

struct subscription
{
  bool isUsed = false;
  int qos = 1;
  String topic;

  subscription() {}
};

struct mqtt
{
  String server;
  int port = 1883;
  String username;
  String password;

  String locationID;
  String deviceID;

  bool enableHeartbeat;

  String ALIVE;
  String HEARRTBEAT;
  String HEARRTBEAT_STATUS;
  String REFRESH;
  String RESETWIFI;

  struct subscription subscriptions[MAX_SUBSCRIPTIONS];
  mqtt(){}
};


class SensorBase {
private:
  // Mosquitto Broker
  mqtt _mqtt;
  int mqttConnectFails = 0;

  char clientID[25];

  WiFiClient _wiFiClient;
  PubSubClient _pubSubClient = PubSubClient(_wiFiClient);
  WiFiUDP _ntpUDP;
  NTPClient _timeClient = NTPClient(_ntpUDP);

  Metro everyTenSeconds = Metro(1000*10);

  void mqttConnect();
  bool publishMQTTHeartbeat();

public:
  SensorBase();

  virtual void begin();
  virtual void loop();

  virtual bool refreshSensor() {};

  char* getClientID();

  WiFiClient getWiFiClient();

  // Mosquitto Broker
  void setMQTT(String server, int port, String username, String password, String locationID, String deviceID);
  void setMQTT(mqtt mqtt);
  bool setMQTTCallback(MQTT_CALLBACK_SIGNATURE);
  bool setMQTTHeartbeat(bool heartbeat);

  bool addMQTTSubscription(String topic, const int qos);
  char* mqtt_callback(char* topic, byte* payload, unsigned int length); // catch what we need

  PubSubClient getPubSubClient();
  bool publishMQTT(String topic, String payload);
  bool publishMQTTStatus(String status);
  bool publishMQTTError(String status);
};


#endif
