#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 30;
int DELAY = 100;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, 15, 15, 15);
  }
  pixels.show(); // Initialize all pixels to ‘off’
}

void loop() {
  int pixel = random(NUMPIXELS);

  for (int i = 15; i < 255; i=i+25) {
    pixels.setPixelColor(pixel, i, 0, 0);
    pixels.show();
    delay(5);
  }
  for (int i = 255; i > 15; i=i-25) {
    pixels.setPixelColor(pixel, i, 0, 0);
    pixels.show();
    delay(5);
  }
  pixels.setPixelColor(pixel, 15, 15, 15);
  pixels.show();
  delay(DELAY);

}
