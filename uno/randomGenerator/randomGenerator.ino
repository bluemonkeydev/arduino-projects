int buttonCount = 4;
int buttons[] = {4,6,8,10};
int leds[] = {7,5,11,9};
int randomNum[] = {-1,-1,-1,-1};

void setup() {
  Serial.begin(9600);
  for (int i=0; i<buttonCount;i++) {
    pinMode(buttons[i], INPUT);
  }
  for (int i=0; i<buttonCount;i++) {
    pinMode(leds[i], OUTPUT);
  }
  blinkAll();
}

int whichButton() {
  while(true) {
    for (int i=0; i<buttonCount; i++) {
      if (digitalRead(buttons[i]) == HIGH) {
        digitalWrite(leds[i], HIGH);
        delay(500);
        digitalWrite(leds[i], LOW);
        return i;
      }
    }
  }
}

void clearAll() {
  for (int i=0; i<buttonCount; i++){
    digitalWrite(leds[i], LOW);
  }
}

void blinkAll() {
  for (int i=0; i<3; i++) {
    digitalWrite(leds[0], HIGH);
    delay(75);
    digitalWrite(leds[1], HIGH);
    delay(75);
    digitalWrite(leds[2], HIGH);
    delay(75);
    digitalWrite(leds[3], HIGH);
    delay(400);
    digitalWrite(leds[3], LOW);
    delay(75);
    digitalWrite(leds[2], LOW);
    delay(75);
    digitalWrite(leds[1], LOW);
    delay(75);
    digitalWrite(leds[0], LOW);
    delay(500);
  }
}

void flashOrder() {
  int minSize = -1;
  int minValue = -1;
  for (int i=0; i<buttonCount; i++) {
    int smallest = -1;
    for (int j=0;j<4; j++) {
      if (randomNum[j] > minSize) {
        if (randomNum[j] < randomNum[(j%4)+1]) {
          smallest = j;
        } else {
          smallest = j+1;
        }
      }
    }
    digitalWrite(leds[smallest], HIGH);
    Serial.println(String(smallest));
    delay(1500);
    minSize = randomNum[smallest];
  }
}

void loop() {
  bool keepLooping = false;
  for(int i=0; i<buttonCount; i++) {
    if (randomNum[i] == -1) {
      keepLooping = true;
      break;
    }
  }
  if (keepLooping) {
    int pressed = whichButton();
    randomNum[pressed] = random(1000);
    Serial.println("button: " + String(pressed) + " was: " + String(randomNum[pressed]));
  } else {
    flashOrder();
    delay(10000);
  }
}

