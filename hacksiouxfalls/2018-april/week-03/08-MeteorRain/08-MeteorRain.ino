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
  meteorRain(0xff,0,0,10, 64, true, 30);
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUMPIXELS+NUMPIXELS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUMPIXELS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUMPIXELS) && (i-j>=0) ) {
        pixels.setPixelColor(i-j, pixels.Color(red, green, blue));
      }
    }
   
    pixels.show();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = pixels.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    pixels.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUMPIXELS; i++ ) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
}
