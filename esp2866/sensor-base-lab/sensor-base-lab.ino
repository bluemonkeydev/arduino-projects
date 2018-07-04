#include <SensorBase.h>

#include <Metro.h>
#include <ESP8266WebServer.h>

// #include "DHTesp.h"
// DHTesp dht;

#define DHTPIN D2
#define DHTTYPE DHT22

#include "DHT.h"
DHT dht(DHTPIN, DHTTYPE);

// Relay Info
const int RELAY_PIN = D3;
const int RELAY_ON  = HIGH;
const int RELAY_OFF = LOW;

// ideal temperature is 74.5F or 23.6C
const float MAX_TEMP_OFF = 30.0; // 85.0;
const float MAX_TEMP_ON  = 18.0; // 65.0;

float lastTemperature = 0.0;
float lastHumidity = 0.0;

String lastHumidity_string = "0.0";
String lastTemperature_string = "0.0";

int temperatureReadFailures = 0;
const int MAX_TEMP_READ_FAILURES = 15;

float relayTempOff = -1.0;
float relayTempOn = -1.0;
boolean relayOn = RELAY_OFF;

// Mosquitto Broker
String MQTT_SERVER = "mqtt.XXXXXXXXXXX.com";
const int MQTT_PORT = 1883;
String MQTT_USERNAME = "XXXXXXXXXXX";
String MQTT_PASSWORD = "XXXXXXXXXXX";

#define MQTT_DEVICE "sensor"
#define MQTT_LOCATION "lab"

String MQTT_IPADDRESS = MQTT_LOCATION "/" MQTT_DEVICE "/ipaddress";
String MQTT_MAC       = MQTT_LOCATION "/" MQTT_DEVICE "/mac";

String MQTT_TEMP     = MQTT_LOCATION "/" MQTT_DEVICE "/temperature";
String MQTT_HUMIDITY = MQTT_LOCATION "/" MQTT_DEVICE "/humidity";

String MQTT_SWITCH         = MQTT_LOCATION "/" MQTT_DEVICE "/switch";
String MQTT_SWITCH_REFRESH = MQTT_LOCATION "/" MQTT_DEVICE "/switch/refresh";
String MQTT_SWITCH_STATUS  = MQTT_LOCATION "/" MQTT_DEVICE "/switch/status";

String MQTT_SWITCH_TEMP_OFF = MQTT_LOCATION "/" MQTT_DEVICE "/switch/off";
String MQTT_SWITCH_TEMP_ON  = MQTT_LOCATION "/" MQTT_DEVICE "/switch/on";

// Thingspeak
String apiKey = "XXXXXXXXXXX";
const char* thingSpeakServer = "api.thingspeak.com";

// Webserver
ESP8266WebServer server( 80 );

const char* WEBSERVER_USERNAME = "admin";
const char* WEBSERVER_PASSWORD = "admin";

const char* pageSettings = "\
<html> \
  <head> \
    <title>Rob's Lab</title> \
    <style> \
    </style> \
  </head> \
  <body> \
    <h1>Rob's Lab</h1> \
    <h2>Status for %s</h2> \
    <dl> \
      <dt><b>Last Temperature</b></dt><dd>%s</dd> \
      <dt><b>Last Humidity</b></dt><dd>%s</dd> \
      <dt><b>Up Time</b><dd>%02d:%02d:%02d:%02d</dd> \
    </dl> \
    <form action=/> \
    <h2>Settings</h2> \
    <dl> \
      <dt><b>Off Temperature</b></dt><dd><input type=input name=offtemp value=%s></dd> \
      <dt><b>On Temperature</b></dt><dd><input type=input name=ontemp value=%s></dd> \
    </dl> \
    <p> \
      <input type=submit value=Save name=save /> \
      <a href=/>Refresh</a> \
    <p> \
    </form> \
  </body> \
</html>"; // device, currTemp, currHum, days, hours, minutes, seconds, offTemp, onTemp, 


class RobsLab : public SensorBase {
  Metro everyHalfSecond = Metro(500);
  Metro everyTenSeconds = Metro(1000*10);
  Metro everyOneMinute = Metro(1000*60);
  Metro everyFiveMinutes = Metro(1000*60*5);

public:
  RobsLab() {}

  virtual void begin() {
    SensorBase::setWifiLED(D4);
    SensorBase::setWifiAPLED(D5);
    
    SensorBase::begin();
    // wait for wifi to be established
    while (WiFi.status() != WL_CONNECTED) {
      delay(250);
    }
    setMQTT(MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, MQTT_LOCATION, MQTT_DEVICE);
    
    // release control to allow the connection to respond
    SensorBase::loop();
    
    IPAddress ip = WiFi.localIP();
    String ipStr = "";
    for (int i=0; i<4; i++) {
      ipStr += i  ? "." + String(ip[i]) : String(ip[i]);
    }
    Serial.print("IP: ");
    Serial.println(ip);

    int maxAttempts = 25;
    while( publishMQTT(MQTT_IPADDRESS, ipStr, true) == false) {
      if (maxAttempts-- < 0) {
        Serial.println("IP FAILED");
        break;
      }
      delay(250);
    }
    maxAttempts = 25;
    while( publishMQTT(MQTT_MAC, WiFi.macAddress(), true) == false) {
      if (maxAttempts-- < 0) {
        Serial.println("MAC FAILED");
        break;
      }
      delay(250);
    }
    Serial.print("Mac: ");
    Serial.println(WiFi.macAddress());

    addMQTTSubscription(MQTT_SWITCH, 1);
    addMQTTSubscription(MQTT_SWITCH_TEMP_OFF, 1);
    addMQTTSubscription(MQTT_SWITCH_TEMP_ON, 1);
    addMQTTSubscription(MQTT_SWITCH_REFRESH, 0);
    
    // Sensor Setup
    digitalWrite(RELAY_PIN, RELAY_OFF);
    pinMode(RELAY_PIN, OUTPUT);
    
    dht.begin();
    // dht.setup(DHTPIN);

    server.on("/", [this](){ webserverRoot();});
    server.onNotFound([this](){ webserverRoot();});
    server.begin();
  }

  // virtual function defined below!
  void webserverRoot() {
    if( !server.authenticate(WEBSERVER_USERNAME, WEBSERVER_PASSWORD)) {
      return server.requestAuthentication();
    }
    if (server.args() > 0) {
      setOnTemp(server.arg("ontemp"));
      setOffTemp(server.arg("offtemp"));
      
      // Push the values back into the queue for persistence
      publishMQTT(MQTT_SWITCH_TEMP_ON, String(relayTempOn), true);
      publishMQTT(MQTT_SWITCH_TEMP_OFF, String(relayTempOff), true);
    }
    
    char page[1200];
    // device, currTemp, currHum, days, hours, minutes, seconds, onTemp, offTemp
    int seconds = millis() / 1000;
    int minutes = seconds / 60;
    int hours = minutes / 60;
    int days = hours / 24;

    String s_lastTemperature = String(lastTemperature);
    String s_lastHumidity = String(lastHumidity);
    String s_relayTempOn = String(relayTempOn);
    String s_relayTempOff = String(relayTempOff);

    snprintf(page, 1200, pageSettings, getClientID(), s_lastTemperature.c_str(), s_lastHumidity.c_str(), \
        days, hours % 24, minutes % 60, seconds % 60, \
        s_relayTempOff.c_str(), s_relayTempOn.c_str());
    server.send(200, "text/html", page);
  }

  void setOnTemp(String onTemp) {
    relayTempOn = onTemp.toFloat();
    
    // data check!
    if (relayTempOn < MAX_TEMP_ON) {
      relayTempOn = -1;
    }
    publishMQTTStatus("Setting temp-on value to: " + String(relayTempOn));
  }

  void setOffTemp(String offTemp) {
    relayTempOff = offTemp.toFloat();
    
    // data check!
    if (relayTempOff > MAX_TEMP_OFF) {
      relayTempOff = -1;
    }
    publishMQTTStatus("Setting temp-off value to: " + String(relayTempOff));
  }

  virtual void loop() {
    SensorBase::loop();
    
    server.handleClient();
    
    if (everyTenSeconds.check()) {
      checkSensor();
      checkRelay();
    }
    
    if (everyOneMinute.check()) {
      logRelay();
    }
    
    if (everyFiveMinutes.check()) {
      refreshSensor();
    }
    
  }

  virtual char* mqtt_callback(char* topic, byte* payload, unsigned int length) {
    char* data = SensorBase::mqtt_callback(topic, payload, length);

    // OVERWRITE the switch (debugging)
    if (strcasecmp(topic, MQTT_SWITCH.c_str()) == 0) {
      Serial.print("switch activated: ");
      Serial.println(data);

      if (strcasecmp(data, "on") == 0 || strcasecmp(data, "true") == 0 || strcmp(data, "1") == 0) {
        digitalWrite(RELAY_PIN, LOW); 
        Serial.println("Switched: Low");
        publishMQTTStatus("Switched: Low(on)");

      } else if (strcasecmp(data, "off") == 0 || strcasecmp(data, "false") == 0 || strcmp(data, "0") == 0) {
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("Switched: High");
        publishMQTTStatus("Switched: High(off)");
       
      } else {
        Serial.println("Switched: Unknown command");
      }

    } else if (strcasecmp(topic, MQTT_SWITCH_TEMP_OFF.c_str()) == 0) {
      Serial.print("temp value: ");
      Serial.println(data);
      
      setOffTemp(String(data));

    } else if (strcasecmp(topic, MQTT_SWITCH_TEMP_ON.c_str()) == 0) {
      Serial.print("temp value: ");
      Serial.println(data);
      
      setOnTemp(String(data));

    } else if (strcasecmp(topic, MQTT_SWITCH_REFRESH.c_str()) == 0) {
      publishMQTTStatus("Check Relay");
      checkRelay();
      logRelay();
    } else {
      Serial.print("NO IDEA WHAT THIS IS: ");
      Serial.println(topic);
      Serial.println(data);
    }
  }
  
  virtual bool refreshSensor() {
    if (isnan(lastHumidity) || isnan(lastTemperature)){
      Serial.println("nan temp or humidity value!!");
      publishMQTTError("refreshSensor(): nan temp or humidity value!!");
      return false;
    }
    
    publishMQTT(MQTT_TEMP, lastTemperature_string.c_str(), true);
    publishMQTT(MQTT_HUMIDITY, lastHumidity_string.c_str(), true);
    
    WiFiClient client = getWiFiClient();
    // Thingspeak
    if (client.connect(thingSpeakServer,80)) { // "184.106.153.149" or api.thingspeak.com
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += lastTemperature_string;
      postStr +="&field2=";
      postStr += lastHumidity_string;
      postStr +="&field3=";
      postStr += String(relayTempOn);
      postStr +="&field4=";
      postStr += String(relayTempOff);
      postStr +="&field5=";
      postStr += String(relayOn);
      postStr += "\r\n\r\n";
      
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
    }
    client.stop();
  }
  
  void setRelay(boolean state) {
    relayOn = state;
    digitalWrite(RELAY_PIN, state);
  }
  
  void logRelay() {
    if (isnan(lastHumidity) || isnan(lastTemperature)){
      Serial.println("nan temp or humidity value!!");
      publishMQTTError("logRelay(): nan temp or humidity value!!");
      return;
    }

    publishMQTT(MQTT_TEMP, lastTemperature_string.c_str(), true);
    publishMQTT(MQTT_HUMIDITY, lastHumidity_string.c_str(), true);
    
    String relayState = "OFF";
    if (relayOn) {
      relayState = "ON";
    }

    Serial.println("relay: " + relayState);
    Serial.println("relayTempOff: " + String(relayTempOff));
    Serial.println("relayTempOn: " + String(relayTempOn));
    Serial.println("currTemp: " + lastTemperature_string);

    publishMQTTStatus("relay: " + relayState);
    publishMQTTStatus("relayTempOff: " + String(relayTempOff));
    publishMQTTStatus("relayTempOn: " + String(relayTempOn));
    publishMQTTStatus("currTemp: " + lastTemperature_string);
  }

  void checkSensor() {
    lastHumidity = dht.readHumidity();
    lastTemperature = dht.readTemperature();
    // float t = dht.toFahrenheit(dht.getTemperature());
    // float h = dht.getHumidity();

    lastHumidity_string = String(lastHumidity, DEC);
    lastTemperature_string = String(lastTemperature, DEC);

    if (isnan(lastHumidity) || isnan(lastTemperature)){
      // Sensor is failing to read, keep track
      temperatureReadFailures++;

      if (temperatureReadFailures >= MAX_TEMP_READ_FAILURES) {
        // oh come-on
        publishMQTTError("checkSensor(): Restarting the device after too many sensor read fails");
        ESP.restart();
      }
    }
    temperatureReadFailures = 0;
  }

  void checkRelay() {
    if (isnan(lastHumidity) || isnan(lastTemperature)){
      Serial.println("nan temp or humidity value!!");
      publishMQTTError("checkRelay(): nan temp or humidity value!!");
      // Saftey precaution, kill the relay if it happens to be on
      setRelay(RELAY_OFF);
      publishMQTT(MQTT_SWITCH_STATUS, "OFF(inactive)");
      return;
    }
    
    if (relayTempOn == -1 || relayTempOn < MAX_TEMP_ON) {
      setRelay(RELAY_OFF);
      publishMQTT(MQTT_SWITCH_STATUS, "OFF(inactive)");
      return;
    }
    if (relayTempOff == -1 || relayTempOff > MAX_TEMP_OFF) {
      setRelay(RELAY_OFF);
      publishMQTT(MQTT_SWITCH_STATUS, "OFF(inactive)");
      return;
    }
    
    if (relayOn == RELAY_ON && lastTemperature > relayTempOff) {
      setRelay(RELAY_OFF);
      publishMQTT(MQTT_HUMIDITY, lastHumidity_string.c_str(), true);
      publishMQTT(MQTT_TEMP, lastTemperature_string.c_str(), true);
      publishMQTT(MQTT_SWITCH_STATUS, "OFF (switched)");
      return;
    }
    if (relayOn == RELAY_OFF && lastTemperature <= relayTempOn) {
      setRelay(RELAY_ON);
      publishMQTT(MQTT_HUMIDITY, lastHumidity_string.c_str(), true);
      publishMQTT(MQTT_TEMP, lastTemperature_string.c_str(), true);
      publishMQTT(MQTT_SWITCH_STATUS, "ON (switched)");
      return;
    }
  }
};

RobsLab robsLab = RobsLab();

void setup() {
  robsLab.begin();
  robsLab.setMQTTCallback(mqtt_callback);
}

void loop() {
  robsLab.loop();
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  robsLab.mqtt_callback(topic, payload, length);
}

