#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 30;
int DELAY = 100;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pixels.begin();
}

long color = pixels.Color(125,125,125);

void loop() {
  for (int q=0; q < 3; q++) {
    for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
      pixels.setPixelColor(i+q, color);
    }
    pixels.show();

    delay(DELAY);

    for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
      pixels.setPixelColor(i+q, 0);
    }
  }
}


