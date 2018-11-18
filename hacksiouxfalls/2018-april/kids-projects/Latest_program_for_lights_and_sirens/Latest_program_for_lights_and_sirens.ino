#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); 
// creates a "virtual" serial port/UART
// connect BT module TX to D10
// connect BT module RX to D11
// connect BT Vcc to 5V, GND to GND
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 5
#define PIN2 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN , NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(8, PIN2 , NEO_GRB + NEO_KHZ800);

void setup()  
{
  // set digital pin to control as an output
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  
  noTone(9);

  // set the data rate for the SoftwareSerial port
  BT.begin(9600);
  // Send test message to other device
  BT.println("Hello from Arduino");
  strip.begin();
  strip2.begin();
  clearLights();
  strip.show();
  strip2.show();
}
char a; // stores incoming character from other device
boolean isTonePlaying = false;
boolean isLightsPlaying = false;
int iTone = 1000;
boolean iToneUp = true;
boolean lightIsBlueRed = true;
unsigned long previousMillis_tone = 0;
unsigned long previousMillis_light = 0;

void loop() 
{
  if (BT.available())
  // if text arrived in from BT serial...
  {
    a=(BT.read());
    if (a=='1')
    {
      digitalWrite(13, HIGH);
      BT.println("Lights & Siren On");
      isTonePlaying = true;
      isLightsPlaying = true;
    }
    if (a=='2')
    {
      digitalWrite(13, LOW);
      BT.println("Lights & Siren Off");
      isLightsPlaying = false;
      isTonePlaying = false;
      clearLights();
      noTone(9);
      iTone = 1000;
    }
    if (a=='?')
    {
      BT.println("Send '1' to turn lights & siren on");
      BT.println("Send '2' to turn lights & siren off");
    }   
  }

  if (isTonePlaying) {
    // The "2" here is the milliseconds between each tone step. Bigger will be slower
    if (millis() - previousMillis_tone >= 2) {
      previousMillis_tone = millis();
      if (iToneUp) {
        iTone++;
        if (iTone > 2000) {
          iToneUp = false;
        }
      } else {
        iTone--;
        if (iTone < 1000) {
          iToneUp = true;
        }
      }
      tone(9,iTone);
    }
  }
  
  if (isLightsPlaying) {
    // The "400" here is the millisecond time between each cycle of Red-Blue/Blue-Red
    if (millis() - previousMillis_light >= 400) {
      previousMillis_light = millis();
      if (lightIsBlueRed) {
        strip.setPixelColor(0, strip.Color(255,0,0));
        strip.setPixelColor(1, strip.Color(255,0,0));
        strip.setPixelColor(2, strip.Color(255,0,0));
        strip.setPixelColor(3, strip.Color(255,0,0));
        strip.setPixelColor(4, strip.Color(0,0,255));
        strip.setPixelColor(5, strip.Color(0,0,255));
        strip.setPixelColor(6, strip.Color(0,0,255));
        strip.setPixelColor(7, strip.Color(0,0,255));
        strip.show();
        strip2.setPixelColor(0, strip.Color(0,0,255));
        strip2.setPixelColor(1, strip.Color(0,0,255));
        strip2.setPixelColor(2, strip.Color(0,0,255));
        strip2.setPixelColor(3, strip.Color(0,0,255));
        strip2.setPixelColor(4, strip.Color(255,0,0));
        strip2.setPixelColor(5, strip.Color(255,0,0));
        strip2.setPixelColor(6, strip.Color(255,0,0));
        strip2.setPixelColor(7, strip.Color(255,0,0));
        strip2.show();
        lightIsBlueRed = false;
      } else {
        strip.setPixelColor(0, strip.Color(0,0,255));
        strip.setPixelColor(1, strip.Color(0,0,255));
        strip.setPixelColor(2, strip.Color(0,0,255));
        strip.setPixelColor(3, strip.Color(0,0,255));
        strip.setPixelColor(4, strip.Color(255,0,0));
        strip.setPixelColor(5, strip.Color(255,0,0));
        strip.setPixelColor(6, strip.Color(255,0,0));
        strip.setPixelColor(7, strip.Color(255,0,0));
        strip.show();
        strip2.setPixelColor(0, strip.Color(255,0,0));
        strip2.setPixelColor(1, strip.Color(255,0,0));
        strip2.setPixelColor(2, strip.Color(255,0,0));
        strip2.setPixelColor(3, strip.Color(255,0,0));
        strip2.setPixelColor(4, strip.Color(0,0,255));
        strip2.setPixelColor(5, strip.Color(0,0,255));
        strip2.setPixelColor(6, strip.Color(0,0,255));
        strip2.setPixelColor(7, strip.Color(0,0,255));
        strip2.show();
        lightIsBlueRed = true;
      }
    }
  }
}

void clearLights() {
  for (int i = 0; i < 8; i++) {
    strip.setPixelColor(i, 0);    //turn every pixel off
    strip2.setPixelColor(i, 0);    //turn every pixel off
  }
  strip.show();
  strip2.show();
}

