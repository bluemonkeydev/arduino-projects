int DELAY = 3500;

void setup() {
  //Serial.begin(9600);
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);
}

void playWinner(int pin) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(pin, HIGH);
    delay(75);
    digitalWrite(pin, LOW);
    delay(75);
  }
  digitalWrite(pin, HIGH);
  delay(DELAY);
  digitalWrite(pin, LOW);
}

void loop() {
  if (digitalRead(0) == HIGH) {
    playWinner(3);
  } else if (digitalRead(2) == HIGH) {
    playWinner(1);
  } else if (digitalRead(4) == HIGH) {
    playWinner(7);
  } else if (digitalRead(6) == HIGH) {
    playWinner(5);
  } else if (digitalRead(8) == HIGH) {
    playWinner(11);
  } else if (digitalRead(10) == HIGH) {
    playWinner(9);
  } 
}



