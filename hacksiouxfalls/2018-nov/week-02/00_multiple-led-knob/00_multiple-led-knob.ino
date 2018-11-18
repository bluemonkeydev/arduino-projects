void setup() {
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  for( int i = 8; i <= 12; i++) {
    digitalWrite(i, LOW);
  }
  int knob = analogRead(A0);
  int light = map(knob, 0, 1024, 0, 6);
  if (light >= 5) {
    digitalWrite(12, HIGH);
  }
  if (light >= 4) {
    digitalWrite(11, HIGH);
  }
  if (light >= 3) {
    digitalWrite(10, HIGH);
  }
  if (light >= 2) {
    digitalWrite(9, HIGH);
  }
  if (light >= 1) {
    digitalWrite(8, HIGH);
  }
  delay(50);
}

