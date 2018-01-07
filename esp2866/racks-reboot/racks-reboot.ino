#include <SensorBase.h>

#include <ESP8266WebServer.h>

// Mosquitto Broker
String MQTT_SERVER = "m14.cloudmqtt.com";
const int MQTT_PORT = 17289;
String MQTT_USERNAME = "[YOUR MQTT_USERNAME]";
String MQTT_PASSWORD = "[YOUR MQTT_PASSWORD]";

#define MQTT_DEVICE "[YOUR DEVICE NAME]"
#define MQTT_LOCATION "[YOUR LOCATION]"

String MQTT_PULSE_TIME = MQTT_LOCATION "/" MQTT_DEVICE "/pulsetime";

String MQTT_COMPUTER = MQTT_LOCATION "/" MQTT_DEVICE "/computer/";

String MQTT_COMPUTER_SUB = MQTT_LOCATION "/" MQTT_DEVICE "/computer/#";
String MQTT_COMPUTER_ALL = MQTT_LOCATION "/" MQTT_DEVICE "/computer/all";

String MQTT_TEST = MQTT_LOCATION "/" MQTT_DEVICE "/test";

// TX / 1
// RX / 3


const unsigned int LATCH_PIN = 3; //RX
const unsigned int CLOCK_PIN = 1; // TX
const unsigned int DATA_PIN = 0;
const unsigned int ME_PIN = 2;
const boolean USE_ME_PIN = false;

// esp12F
//const unsigned int LATCH_PIN = 16;
//const unsigned int CLOCK_PIN = 14;
//const unsigned int DATA_PIN = 12;
//const unsigned int ME_PIN = 13;

const unsigned int D_CLEAR = 0;
const unsigned int D_ALL = 0xFFFF;
int pulseDelay = 1000;

class Racks : public SensorBase {

public:
  Racks() {}

  virtual void begin() {
    SensorBase::begin();

    digitalWrite(LATCH_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(DATA_PIN, LOW);
    if (USE_ME_PIN) {
      digitalWrite(ME_PIN, HIGH);
    }
    
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    if (USE_ME_PIN) {
      pinMode(ME_PIN, OUTPUT);
    }
    
    pulse(D_CLEAR);

    if (USE_ME_PIN) {
      digitalWrite(ME_PIN, LOW);
    }
    
    setMQTT(MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, MQTT_LOCATION, MQTT_DEVICE);
    
    addMQTTSubscription(MQTT_COMPUTER_SUB, 1);
    addMQTTSubscription(MQTT_COMPUTER_ALL, 1);
    addMQTTSubscription(MQTT_PULSE_TIME, 1);
    
    // DEBUGGING ONLY
    // addMQTTSubscription(MQTT_TEST, 0);
  }

  virtual void loop() {
    SensorBase::loop();
  }

  virtual bool refreshSensor() {
    publishMQTTStatus("pulsetime: " + String(pulseDelay));
  }
  
  virtual char* mqtt_callback(char* topic, byte* payload, unsigned int length) {
    char* data = SensorBase::mqtt_callback(topic, payload, length);

    int computerOffset = MQTT_COMPUTER.length();
    
    if (stricmp(topic, MQTT_COMPUTER_ALL.c_str()) == 0) {
      if (stricmp(data, "toggle") == 0 || stricmp(data, "on") == 0) {
        shiftTwoBytes(D_ALL);
        publishMQTTStatus("Pulsed all computers");
        publishMQTT(MQTT_COMPUTER_ALL, "off");
      }
    } else if (strncmp(topic, MQTT_COMPUTER.c_str(), computerOffset) == 0) {
      char* computer_str = topic+computerOffset;
      int computer = atoi(computer_str);
      
      if (stricmp(data, "on") == 0) {
        if (computer >= 0 && computer <= 16) {
          pulse(computer);
          publishMQTTStatus("Pulsed: " + String(computer_str));
          publishMQTT(MQTT_COMPUTER + computer_str, "off");
        } else {
          publishMQTTError("Unknown computer: " + String(computer_str));
        }
      }
    } else if (stricmp(topic, MQTT_PULSE_TIME.c_str()) == 0) {
      int pulseDelay_new = atoi(data);

      if (pulseDelay_new >= 100 || pulseDelay_new <= 15000) {
        pulseDelay = pulseDelay_new;
        publishMQTTStatus("New pulse day value: " + String(pulseDelay));
      } else {
        publishMQTTStatus("Unknown or bad pulse time: " + String(pulseDelay));
      }
    } else if (stricmp(topic, MQTT_TEST.c_str()) == 0) {
      publishMQTTStatus("Running test");
      test();
    }
  }

  void pulse(int pin) {
    unsigned int pin_id = 0;
    if (pin < 0 || pin > 16 ) {
      return;
    }
    if (pin > 0) {
      pin_id = 1;
      for (int i = 1; i < pin; i++ ) {
        pin_id *= 2;
      }
    }
    shiftTwoBytes(pin_id);
  }
  
  void shiftTwoBytes(unsigned int bytes){
    // set the bits
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, (bytes >> 8));
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, (bytes));
    digitalWrite(LATCH_PIN, HIGH);
  
    if (bytes > 0) {
      delay(pulseDelay);

      // now clear the bit
      digitalWrite(LATCH_PIN, LOW);
      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, (D_CLEAR >> 8));
      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, (D_CLEAR));
      digitalWrite(LATCH_PIN, HIGH);
    }
  }

  void test() {
  
    for (int i = 1; i <= 16; i++) {
      pulse(i);
      delay(100);
    }
    pulse(0);
  
    for (int i = 1; i <=16; i++) {
      pulse(random(1,17));
      delay(100);
    }
  }
};

Racks racks = Racks();

void setup() {
  racks.begin();
  racks.setMQTTCallback(mqtt_callback);
}

void loop() {
  racks.loop();
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  racks.mqtt_callback(topic, payload, length);
}

