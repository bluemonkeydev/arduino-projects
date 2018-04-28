#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 30;
int DELAY = 100;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pixels.begin();
  pixels.show(); // Initialize all pixels to ‘off’
}

void loop() {
  float rate = 0.03;
  int cycles = 100;
  int wait = 2;
  float pos=0.0;
  // cycle through x times
  for(int x=0; x<(pixels.numPixels()*cycles); x++)
  {
      pos = pos+rate;
      for(int i=0; i<pixels.numPixels(); i++) {
        // sine wave, 3 offset waves make a rainbow!
        float level = sin(i+pos) * 127 + 128;
        // set color level 
        pixels.setPixelColor(i,0,0,(int)level);
      }
         pixels.show();
         delay(wait);
  }
}
