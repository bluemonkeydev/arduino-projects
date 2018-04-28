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
  int r = 255;
  int g = 96;
  int b = 12;
  
  for(int x = 0; x <30; x++)
  {
    int flicker = random(0,40);
    int r1 = r-flicker;
    int g1 = g-flicker;
    int b1 = b-flicker;
    if(g1<0) g1=0;
    if(r1<0) r1=0;
    if(b1<0) b1=0;
    pixels.setPixelColor(x,r1,g1, b1);
  }
  pixels.show();
  delay(random(50,150));
}
