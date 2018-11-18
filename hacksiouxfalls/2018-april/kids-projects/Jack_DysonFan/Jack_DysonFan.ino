const int FAN = 10;
const int LED = 9;
const int CONTROL = A0;

int fanSpeed = 0;
int fadeAmount = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(CONTROL, INPUT);
  Serial.begin(9600);
}

void loop() {
  int fanSpeed = analogRead(CONTROL);

  fanSpeed = map(fanSpeed, 0, 1024, 0, 255);

  Serial.println(fanSpeed);

  analogWrite(LED, fanSpeed);
  analogWrite(FAN, fanSpeed);

  delay(250);
}

