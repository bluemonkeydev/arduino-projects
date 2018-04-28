#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 30;
int DELAY = 25;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

long color = pixels.Color(125,45,0);
long dotColor = pixels.Color(125,0,0);

int dot = NUMPIXELS/2;
int goingLeft = 0;
int loops = 0;

void setup() {
  pixels.begin();
  for( int i = 0; i <= NUMPIXELS; i++) {
    // clear old ones
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}


void loop() {
  loops++;
  if (loops % random(10, 20) == 0) {
    goingLeft = !goingLeft;
  }
  pixels.setPixelColor(dot, color);
  if (goingLeft == 1) {
    if (dot < NUMPIXELS) {
      dot++;
    } else {
      goingLeft = 0;
    }
  } else {
    if (dot > 0) {
      dot--;
    } else {
      goingLeft = 1;
    }
  }
  pixels.setPixelColor(dot, dotColor);
  pixels.show();
  delay(DELAY);
}
