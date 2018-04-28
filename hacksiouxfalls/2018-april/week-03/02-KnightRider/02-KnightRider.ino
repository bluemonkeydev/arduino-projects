#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 30;
int DELAY = 35;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  pixels.begin();
}

void loop() {
  
  for( int i = 0; i < NUMPIXELS; i++) {
    // clear old ones
    pixels.setPixelColor(i-3, pixels.Color(0,0,0));
    pixels.setPixelColor(i-2, pixels.Color(0,0,0));
    // set leading, main and trailing
    pixels.setPixelColor(i-2, pixels.Color(25,0,0));
    pixels.setPixelColor(i-1, pixels.Color(100,0,0));
    pixels.setPixelColor(i, pixels.Color(255,75,75));
    pixels.setPixelColor(i+1, pixels.Color(100,0,0));
    pixels.setPixelColor(i+2, pixels.Color(25,0,0));
    pixels.show();
    delay(DELAY);
  }
  for( int i = NUMPIXELS; i > 0; i--) {
    // clear old ones
    pixels.setPixelColor(i+3, pixels.Color(0,0,0));
    pixels.setPixelColor(i+2, pixels.Color(0,0,0));
    // set leading, main and trailing
    pixels.setPixelColor(i-2, pixels.Color(25,0,0));
    pixels.setPixelColor(i-1, pixels.Color(100,0,0));
    pixels.setPixelColor(i, pixels.Color(255,75,75));
    pixels.setPixelColor(i+1, pixels.Color(100,0,0));
    pixels.setPixelColor(i+2, pixels.Color(25,0,0));
    pixels.show();
    delay(DELAY);
  }

}
