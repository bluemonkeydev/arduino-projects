//buzzer
void setup() {
  pinMode(11, INPUT);
  pinMode(9, OUTPUT);
}

int toneValue = 1000;
boolean toneGoingUp = false;
void loop() {
  int readSwitch = digitalRead(11);
  if (readSwitch == LOW) {
    // pushed
    if (toneGoingUp) {
      toneValue++;
      if (toneValue > 2000) {
        toneGoingUp = false;
      }
    } else {
      toneValue --;
      if (toneValue < 1000) {
        toneGoingUp = true;
      }
    }
    tone(9, toneValue);
  } else {
    noTone(9);
  }
}
