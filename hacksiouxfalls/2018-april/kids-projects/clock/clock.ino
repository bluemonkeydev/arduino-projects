#include <Adafruit_NeoPixel.h>

int PIN = 6;
int NUMPIXELS = 60;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int HOUR_UP = 9;
int MIN_UP = 8;

void setup() {
  pixels.begin();
  pixels.show();

  pinMode(HOUR_UP, INPUT);
  pinMode(MIN_UP, INPUT);

}

int hour = 0;
int minute = 0;
int second = 0;

void loop() {

  int hour_up = digitalRead(HOUR_UP);
  int min_up = digitalRead(MIN_UP);

  pixels.setPixelColor(second, 0, 0, 0);
  pixels.setPixelColor(hour, 0, 0, 0);
  pixels.setPixelColor(minute, 0, 0, 0);

  if (hour_up == HIGH) {
    hour ++;
  }
  if (min_up == HIGH) {
    minute ++;
  }

  second++;
  if (second == 60) {
    minute++;
    second = 0;
  }
  if (minute == 60) {
    hour = hour + 5;
    minute = 0;
  }

  if (hour == 60) {
    hour = 0;
  }

  pixels.setPixelColor(second, 255, 0, 0);
  pixels.setPixelColor(hour, 0, 255, 0);
  pixels.setPixelColor(minute, 0, 0, 255);

  if (second == minute && minute == hour) {
    pixels.setPixelColor(minute, 255, 255, 255);
  } else if (minute == hour) {
    pixels.setPixelColor(minute, 255, 255, 0);
  } else if (second == minute) {
    pixels.setPixelColor(minute, 255, 0, 255);
  } else if (second == hour) {
    pixels.setPixelColor(second, 243, 182, 15);
  }
  
  pixels.show();

  delay(1000);

}
