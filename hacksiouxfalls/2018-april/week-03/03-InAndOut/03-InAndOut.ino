#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 30;
int DELAY = 35;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  pixels.begin();
}

void loop() {
  int half = NUMPIXELS / 2;

  long color = pixels.Color(125,45,0);
  for( int i = 0; i <= half; i++) {
    // clear old ones
    pixels.setPixelColor(i, color);
    pixels.setPixelColor(NUMPIXELS-i, color);
    pixels.show();
    delay(DELAY);
  }
  delay(150);
  
  long color1 = pixels.Color(0,255,40);
  long color2 = pixels.Color(0,150, 20);
  for( int i = half; i > 0; i--) {
    // clear old ones
    pixels.setPixelColor(i+2, color);
    pixels.setPixelColor(i+1, color1);
    pixels.setPixelColor(i, color2);
    pixels.setPixelColor(NUMPIXELS-i, color2);
    pixels.setPixelColor(NUMPIXELS-i-1, color1);
    pixels.setPixelColor(NUMPIXELS-i-2, color);
    pixels.show();
    delay(DELAY+10);
  }
  pixels.setPixelColor(0, color);
  pixels.setPixelColor(1, color);
  pixels.setPixelColor(2, color);
  pixels.setPixelColor(NUMPIXELS-1, color);
  pixels.setPixelColor(NUMPIXELS-2, color);
  pixels.setPixelColor(NUMPIXELS-3, color);
  
  color = pixels.Color(0,0,0);
  for( int i = half; i > 0; i--) {
    // clear old ones
    pixels.setPixelColor(i, color);
    pixels.setPixelColor(NUMPIXELS-i, color);
    pixels.show();
    delay(DELAY);
  }

}
