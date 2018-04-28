#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 30;
int DELAY = 25;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  pixels.begin();
}

void loop() {
    // pick a random color each time
  int r = random(150);
  int g = random(150);
  int b = random(150);
  
  for( int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r,g,b));
    pixels.show();
    delay(DELAY);
  }

  r = random(150);
  g = random(150);
  b = random(150);
  
  for( int i = NUMPIXELS; i > 0; i--) {
    pixels.setPixelColor(i, pixels.Color(r,g,b));
    pixels.show();
    delay(DELAY);
  }

}
