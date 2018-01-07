#include "SensorBase.h"

SensorBase::SensorBase() {

}

void SensorBase::begin() {

  Serial.begin(115200);

  // set ESP client name
  uint32_t chipid=ESP.getChipId();
  snprintf(clientID,25,"esp_%06X",chipid);

  Serial.print("ClientID: ");
  Serial.println(clientID);

  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.autoConnect(clientID);

  Serial.println("connected...yeey :)");
  
  WiFi.mode(WIFI_STA);

    // adjust time
  _timeClient.setTimeOffset((-6)*60*60);
  _timeClient.setUpdateInterval(60*60*1000); //hourly
  _timeClient.begin();
}

void SensorBase::loop() {
  if (!_pubSubClient.connected()) {
    mqttConnect();
  }
  _pubSubClient.loop();
  _timeClient.update();

  if (everyTenSeconds.check()) {
    publishMQTTHeartbeat();
  }
}

char* SensorBase::getClientID() {
  return clientID;
}

WiFiClient SensorBase::getWiFiClient() {
  return _wiFiClient;
}

void SensorBase::setMQTT(String server, int port, String username, String password, String locationID, String deviceID) {
  _mqtt.server = server.c_str();
  _mqtt.port = port;
  _mqtt.username = username.c_str();
  _mqtt.password = password.c_str();

  if (locationID.length() == 0) {
    locationID = "no-location";
  }
  _mqtt.locationID = locationID.c_str();

  if (deviceID.length() == 0) {
    deviceID = "no-device";
  }
  _mqtt.deviceID = deviceID.c_str();
  
  // make a few topics we'll use
  // Leave as "String" values because the c_str() method returns a const char*
  _mqtt.ALIVE = String(locationID + "/" + deviceID + "/alive");
  _mqtt.HEARRTBEAT = String(locationID + "/" + deviceID + "/heartbeat");
  _mqtt.HEARRTBEAT_STATUS = String(locationID + "/" + deviceID + "/heartbeat/status");
  _mqtt.REFRESH = String(locationID + "/" + deviceID + "/refresh");
  _mqtt.RESETWIFI = String(locationID + "/" + deviceID + "/reset-wifi");
  
  _pubSubClient.setServer(_mqtt.server.c_str(), _mqtt.port);
  mqttConnect();

}

void SensorBase::setMQTT(mqtt mqtt){
  setMQTT(mqtt.server, mqtt.port, mqtt.username, mqtt.password, mqtt.locationID, mqtt.deviceID);
}

bool SensorBase::setMQTTCallback(MQTT_CALLBACK_SIGNATURE) {
  _pubSubClient.setCallback(callback);
}

bool SensorBase::addMQTTSubscription(String topic, const int qos) {
  bool sub = false;

  for(int i = 0; i < MAX_SUBSCRIPTIONS; i++){
    if(_mqtt.subscriptions[i].isUsed == false){
      _mqtt.subscriptions[i].topic = topic;
      _mqtt.subscriptions[i].qos = qos;
      _mqtt.subscriptions[i].isUsed = true;
      sub = true;
      break;
    }
  }

  if (sub == true) {
    _pubSubClient.subscribe(topic.c_str(), qos);
  }
  
  return sub;
}

char* SensorBase::mqtt_callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  char* data = new char[length+1];

  // Convert to char*
  for (int i = 0; i < length; i++) {
    data[i] = (char)payload[i];
  }
  data[length] = 0;

  if (stricmp(topic, _mqtt.RESETWIFI.c_str()) == 0) {
    Serial.println("resetting wifi");
    publishMQTTStatus("Resetting wifi & launching portal");
    
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    wifiManager.startConfigPortal(clientID);
    Serial.println("connected...yeey :)");

  } else if (stricmp(topic,_mqtt.REFRESH.c_str()) == 0) {
    Serial.print("refresh activated: ");
    Serial.println(data);
    // publishMQTTStatus("Sending Data");

    refreshSensor();
  } else if (stricmp(topic, _mqtt.HEARRTBEAT.c_str()) == 0) {
    Serial.print("heartbeat switched: ");
    Serial.println(data);

    // convert to lower
    for( int i = 0; i < length; i++) {
      data[i] = tolower(data[i]);
    }
    if (strncmp(data, "true", 4) == 0) {
      setMQTTHeartbeat(true);
    } else {
      setMQTTHeartbeat(false);
    }
    // publishMQTTStatus("updated heartbeat");
  }

  return data;
}


void SensorBase::mqttConnect() {
  if (!_pubSubClient.connected()) {
    _pubSubClient.setServer(_mqtt.server.c_str(), _mqtt.port);

    Serial.print("Attempting MQTT connection...");

    if (_pubSubClient.connect(clientID, _mqtt.username.c_str(), _mqtt.password.c_str())) {
      Serial.println("connected");
      publishMQTTStatus("is Alive");

      _pubSubClient.subscribe(_mqtt.REFRESH.c_str(), 0);
      _pubSubClient.subscribe(_mqtt.RESETWIFI.c_str(), 0);
      _pubSubClient.subscribe(_mqtt.HEARRTBEAT.c_str(), 0);

      for(int i = 0; i < MAX_SUBSCRIPTIONS; i++){
        if(_mqtt.subscriptions[i].isUsed == true){
          _pubSubClient.subscribe(_mqtt.subscriptions[i].topic.c_str(), _mqtt.subscriptions[i].qos);
        }
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(_pubSubClient.state());
      Serial.println(" try again in 5 seconds");
      publishMQTTStatus("Attempted MQTT connection (FAILED state: " + String(_pubSubClient.state()) +")");
      //delay(5000);
    }
  }
}


PubSubClient SensorBase::getPubSubClient() {
  return _pubSubClient;
}

bool SensorBase::setMQTTHeartbeat(bool beat) {
  bool last = _mqtt.enableHeartbeat;
  _mqtt.enableHeartbeat = beat;
  return last;
}

bool SensorBase::publishMQTTHeartbeat() {
  if(_pubSubClient.connected() && _mqtt.enableHeartbeat) {
    return _pubSubClient.publish(_mqtt.HEARRTBEAT_STATUS.c_str(), _timeClient.getFormattedTime().c_str());
  }
  return false;
}

bool SensorBase::publishMQTT(String topic, String payload) {
  if(_pubSubClient.connected()) {
    return _pubSubClient.publish(topic.c_str(), payload.c_str());
  }
  return false;
}

bool SensorBase::publishMQTTStatus(String status) {
  if(_pubSubClient.connected()) {
    return _pubSubClient.publish(_mqtt.ALIVE.c_str(), String("INFO: " + String(clientID) + " " + status + "(" + _timeClient.getFormattedTime() + ")").c_str());
  }
  return false;
}

bool SensorBase::publishMQTTError(String status) {
  if(_pubSubClient.connected()) {
    return _pubSubClient.publish(_mqtt.ALIVE.c_str(), String("ERROR: " + String(clientID) + " " + status + "(" + _timeClient.getFormattedTime() + ")").c_str());
  }
  return false;
}
