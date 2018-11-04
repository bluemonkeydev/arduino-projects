const int SPEAKER_PIN = 9;
const int LED_PIN = 10;
const int BUTTON_PIN = 11;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

boolean isTonePlaying = false;
int iTone = 1000;
boolean iToneUp = true;
unsigned long previousMillis_tone = 0;
unsigned long startMillis_tone = 0;

boolean isLedBlinking = false;
boolean isLedOn = true;
unsigned long previousMillis_led = 0;
unsigned long startMillis_led = 0;

void loop() {
  int button = digitalRead(BUTTON_PIN);
  if (button == LOW) {
    isTonePlaying = false;
    isLedBlinking = false;
    digitalWrite(LED_PIN, LOW);
    noTone(9);
    delay(5000);
  }
  
  int vibs = analogRead(A0);
  
  int level = map(vibs, 400, 1024, 0, 100);

  Serial.print("level: ");
  Serial.println(level);

  if (level > 90) {
    isTonePlaying = true;
    isLedBlinking = true;
  } else if (level > 40) {
    isLedBlinking = true;
  }

  if (isLedBlinking) {
    if (millis() - previousMillis_led >= 500) {
      previousMillis_led = millis();
      isLedOn = !isLedOn;
      digitalWrite(LED_PIN, isLedOn);
    }
  }
  
  if (isTonePlaying) {
    // The "2" here is the milliseconds between each tone step. Bigger will be slower
    if (millis() - previousMillis_tone >= 2) {
      previousMillis_tone = millis();
      if (iToneUp) {
        iTone++;
        if (iTone > 2000) {
          iToneUp = false;
        }
      } else {
        iTone--;
        if (iTone < 1000) {
          iToneUp = true;
        }
      }
      tone(SPEAKER_PIN,iTone);
    }
  }
}

