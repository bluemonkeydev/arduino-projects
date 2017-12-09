#include "Arduino.h"

const int LED_PIN = 13;
const int SENSOR_PIN = 7;
const int SIGNAL_OUTPUT_PIN = 6;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(SIGNAL_OUTPUT_PIN, OUTPUT);
}

unsigned long onMillis = 0;
unsigned long offMillis = 0;

const int onInterval = 5*1000;   // 5 seconds
const int offInterval = 10*1000; // 10 seconds

const float highToLowThreashold = 0.10;
long onHighCount = 0;
long onLowCount = 0;

boolean onTimer = false;
boolean offTimer = false;

boolean vibration = false;

void loop() {
  unsigned long currentMillis = millis();
  int newSensorVal = digitalRead(SENSOR_PIN);
  digitalWrite(SIGNAL_OUTPUT_PIN, newSensorVal);
  if (newSensorVal == HIGH) {
    offTimer = true;  // make sure timer's on
    offMillis = currentMillis; // reset timer
    if (onTimer == false && vibration == false) {
      // start on-timer
      onMillis = currentMillis;
      onTimer = true;
      onHighCount = 0;
      onLowCount = 0;
    }
  }

  // track on-timer high-pulses
  if (vibration == false && onTimer == true) {
    if (newSensorVal == HIGH) {
      onHighCount++;
    } else {
      onLowCount++;
    }
  }
  
  if (vibration == false && onTimer == true && 
        (currentMillis - onMillis >= onInterval)) {
    // On Timer triggered
    float avgCount = (float)onHighCount / (float)(onHighCount+onLowCount);
    Serial.println("on timer: FIRED");
    Serial.println("values: " + String(onHighCount) + ":" + String(onLowCount) + " avg: " + String(avgCount));
    onTimer = false;
    onHighCount = 0;
    onLowCount = 0;
    if (avgCount > highToLowThreashold) {
      vibration = true;
      digitalWrite(LED_PIN, HIGH);
    } else {
      Serial.println("threashold not met");
    }
  }

  if (offTimer == true && (currentMillis - offMillis >= offInterval)) {
    // off trigger
    Serial.println("off timer: FIRED");
    vibration = false;
    offTimer = false;
    digitalWrite(LED_PIN, LOW);
  }

}


