#include <Adafruit_NeoPixel.h>
#define PIN 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN , NEO_GRB + NEO_KHZ800);

void setup()  
{
  strip.begin();
  clearLights();
  strip.show();
}

long previousMillis_light = 0;
boolean lightIsBlueRed = false;

void loop() 
{
  // The "400" here is the millisecond time between each cycle of Red-Blue/Blue-Red
  if (millis() - previousMillis_light >= 400) {
    previousMillis_light = millis();
    if (lightIsBlueRed) {
      strip.setPixelColor(0, strip.Color(255,0,0));
      strip.setPixelColor(1, strip.Color(255,0,0));
      strip.setPixelColor(2, strip.Color(255,0,0));
      strip.setPixelColor(3, strip.Color(75,75,75));
      strip.setPixelColor(4, strip.Color(75,75,75));
      strip.setPixelColor(5, strip.Color(75,75,75));
      strip.setPixelColor(6, strip.Color(75,75,75));
      strip.setPixelColor(7, strip.Color(75,75,75));
      strip.show();
      lightIsBlueRed = false;
    } else {
      strip.setPixelColor(0, strip.Color(75,75,75));
      strip.setPixelColor(1, strip.Color(75,75,75));
      strip.setPixelColor(2, strip.Color(75,75,75));
      strip.setPixelColor(3, strip.Color(75,75,75));
      strip.setPixelColor(4, strip.Color(75,75,75));
      strip.setPixelColor(5, strip.Color(0,0,255));
      strip.setPixelColor(6, strip.Color(0,0,255));
      strip.setPixelColor(7, strip.Color(0,0,255));
      strip.show();
      lightIsBlueRed = true;
    }
  }
}

void clearLights() {
  for (int i = 0; i < 8; i++) {
    strip.setPixelColor(i, 0);    //turn every pixel off
  }
  strip.show();
}

