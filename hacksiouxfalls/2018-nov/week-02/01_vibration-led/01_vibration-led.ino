// Vibration
void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);
}

void loop() {
  int shake = analogRead(A0);
  Serial.println(shake);
  if (shake > 500) {
    digitalWrite(10, HIGH);
  } else {
    digitalWrite(10, LOW);
  }
  delay(500);
}
