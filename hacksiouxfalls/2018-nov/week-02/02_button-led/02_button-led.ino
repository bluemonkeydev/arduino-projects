// Switch
void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
}

void loop() {
  int readSwitch = digitalRead(11);
  if (readSwitch == LOW) {
    digitalWrite(10, HIGH);
  } else {
    digitalWrite(10, LOW);
  }
}
