#include <LowPower.h>

const int led = 12;
const int ledGreen = 11;
const int wakeUpPin = 2;

void setup()
{
  Serial.begin(9600);
    // Initialize the IO and ISR
//    vw_set_ptt_inverted(true); // Required for DR3100
//    vw_set_tx_pin(8);
//    vw_setup(2000);  // Bits per sec
    pinMode(led, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(wakeUpPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, LOW);
}

void blinkLed(){
  digitalWrite(led, HIGH);
  delay(250);  
  digitalWrite(led, LOW);
  delay(250);  
  digitalWrite(led, HIGH);
  delay(250);  
  digitalWrite(led, LOW);
}

void blinkGreenLed(){
  delay(500);  
  digitalWrite(ledGreen, HIGH);
  delay(250);  
  digitalWrite(ledGreen, LOW);
}

int blinkGreen = 0;
// (8 sec * 7.5) [min] * 60 min * 8 hours = 3600
// (8 sec * 7.5) [min] * 60 min * 4 hours = 1800
const int DEEP_SLEEP_AFTER = 1800;
int runForUntilDeepSleep = DEEP_SLEEP_AFTER; // 8 second increments

void wakeUp() {
  // nothing to do
  blinkGreenLed();
}

void loop()
{
    runForUntilDeepSleep--;
    if (runForUntilDeepSleep <= 0) {
      digitalWrite(wakeUpPin, HIGH);
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
      // Coming back from sleep
      runForUntilDeepSleep = DEEP_SLEEP_AFTER;
    }
    if (blinkGreen++ > 3) {
      blinkGreenLed();
      blinkGreen = 0;
    } else {
       blinkLed();
    }
    digitalWrite(wakeUpPin, HIGH);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    //sleepTenMinutes(); 
}
