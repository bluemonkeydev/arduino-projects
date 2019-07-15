int DELAY = 3500;
int LED_DELAY = 250;

int buttonCount = 4;

int leds[] = {3,5,7,9};
int buttons[] = {2,4,6,8};

int pattern[] = {0,2,3,1,1,2,3,0,0,1};
int level = 1;
int maxLevel = 10;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < buttonCount; i++) {
    pinMode(leds[i], OUTPUT);
  }
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttons[i], INPUT);
  }
  testButtons();
  blinkAll();
}

void testButtons() {
  while(true) {
    for (int i = 0; i < buttonCount; i++) {
      int state = digitalRead(buttons[i]);
      if (state == HIGH) {
        digitalWrite(leds[i], HIGH);
        delay(100);
        digitalWrite(leds[i], LOW);
      }
    }
  }
}

int whichButton() {
  while(true) {
    for (int i=0; i<buttonCount; i++) {
      if (digitalRead(buttons[i]) == HIGH) {
        digitalWrite(leds[i], HIGH);
        delay(LED_DELAY);
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

void showPattern() {
  clearAll();
  Serial.print("Pattern: " );
  delay(800);
  for (int i = 0; i < level; i++ ){
    Serial.print(String(pattern[i]) + " ");
    digitalWrite(leds[pattern[i]], HIGH);
    delay(LED_DELAY);
    digitalWrite(leds[pattern[i]], LOW);
    delay(LED_DELAY);
  }
  Serial.println();
}

bool playGame() {
  if (level == maxLevel) {
    // you won!
    blinkAll();
    level = 0;
  }
  for (int i=0; i<level; i++ ){
    Serial.println("***********");
    Serial.println("level:   " + String(level));
    int pressed = whichButton();
    Serial.println("pressed: " + String(pressed));
    Serial.println("expect:  " + String(pattern[i]));
    if (pressed != pattern[i]) {
      // MISSED
      blinkAll();
      return false;
    }
  }
  return true;
}

void loop() {
  showPattern();
  if (playGame() == false) {
    // missed; start over
    level = 0;
  } else {
    // good, next level
    level++;
  }
}
