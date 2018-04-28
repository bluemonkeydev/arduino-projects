#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 30;
int DELAY = 100;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pixels.begin();
  pixels.show(); // Initialize all pixels to ‘off’
}
int lights = 0;
void loop() {
  int vibs = analogRead(A0);
  Serial.print("val: ");
  //delay(50);
  Serial.println(vibs);
  
  lights = map(vibs, 250, 1024, 0, NUMPIXELS);
  Serial.print("lit: ");
  Serial.println(lights);
  for(int i = 0; i < NUMPIXELS; i++) {
    if (i < lights) {
      pixels.setPixelColor(i, pixels.Color(110,0,0));
    } else {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
    
  }
  pixels.show();
}

