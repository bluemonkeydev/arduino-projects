const int LED = 2;
const int BUTTON = 3;
const int BUZZER = 11;

const int SHORT = 250;
const int LONG = 750;

const int SHORT_TONE = 800;
const int LONG_TONE = 1000;

const int USER_TONE = 3000;
const int BAD_TONE = 100;

int thisLevel = 1;
const int maxLevel = 13;
int pattern[maxLevel];

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT_PULLUP);
}

void makePattern() {
  randomSeed(analogRead(0));
  Serial.println("The Pattern");
  for (int i = 0; i < maxLevel; i++ ){
    pattern[i] = random(1,3); // pick 1 or 2
    Serial.print(String(pattern[i]) +" ");
  }
  Serial.println();
}

void showPattern() {
  Serial.println("LEVEL: " + String(thisLevel));
  for (int i = 0; i < thisLevel; i++ ){
    if (pattern[i] == 1) {
      Serial.println("SHORT");
      digitalWrite(LED, HIGH);
      tone(BUZZER, SHORT_TONE);
      delay(SHORT);
      digitalWrite(LED, LOW);
      noTone(BUZZER);
    } else if (pattern[i] == 2) {
      Serial.println("LONG");
      digitalWrite(LED, HIGH);
      tone(BUZZER, LONG_TONE);
      delay(LONG);
      digitalWrite(LED, LOW);
      noTone(BUZZER);
    } else {
      return;
    }
    delay(LONG); // time between
  }
}

int readGuess() {
  while(digitalRead(BUTTON) == HIGH) {
    delay(5);
  }
  long startTime = millis();
  delay(40); // button debounce
  
  digitalWrite(LED, HIGH);
  tone(BUZZER, USER_TONE);
  while(digitalRead(BUTTON) == LOW) {
    delay(25); // button debounce
  }
  digitalWrite(LED, LOW);
  noTone(BUZZER);
  long seconds = millis() - startTime;
  
  if (seconds > SHORT) {
    return LONG;
  } else {
    return SHORT;
  }
}

boolean guessPattern() {
  for (int i = 0; i < thisLevel; i++ ){
    int guess = readGuess();
    
    if (guess == SHORT && pattern[i] == 1) {
      // yeah!
      Serial.println("SHORT: correct guess");
    } else if (guess == LONG && pattern[i] == 2) {
      // yeah!
      Serial.println("LONG: correct guess");
    } else {
      // boo, bad guess
      String guessStr = "LONG";
      if (guess == SHORT) {
        guessStr = "SHORT";
      }
      String answer = "LONG";
      if (pattern[i] == 1) {
        answer = "SHORT";
      }
      Serial.println("WRONG: guessed " + guessStr + " but was " + answer);
      return false;
    }
  }
  return true;
}
void blinkButton(int times){
  for( int i = 0; i < times; i++) {
    digitalWrite(LED, HIGH);
    tone(BUZZER, USER_TONE);
    delay(100);
    digitalWrite(LED, LOW);
    noTone(BUZZER);
    delay(100);
  }
}

void loop() {
  Serial.println("Make the pattern");
  makePattern();
  Serial.println("Let's start");
  blinkButton(5);
  boolean isCorrect = true;
  thisLevel = 1;
  while( isCorrect ) {
    delay(1500);
    Serial.println("Show pattern");
    showPattern();
    Serial.println("Start guessing");
    Serial.println("Level: " + String(thisLevel));
    isCorrect = guessPattern();
    thisLevel++;
  }
  tone(BUZZER, BAD_TONE);
  blinkButton(2);
  delay(1500);
  noTone(BUZZER);
}
