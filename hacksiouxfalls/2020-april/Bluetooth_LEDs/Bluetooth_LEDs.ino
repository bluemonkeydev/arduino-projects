
#include <SoftwareSerial.h>
SoftwareSerial BT(A0, A1); 

void setup() {
  Serial.begin(9600);

  BT.begin(9600);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
  if (BT.available())
  {
    char command = BT.read();
    Serial.println(command);
    if (command == '1')
    {
      digitalWrite(10, HIGH);

    } else if (command == '2') {
      digitalWrite(11, HIGH);

    } else if (command == '3') {
      digitalWrite(12, HIGH);

    } else if (command == '4') {
      digitalWrite(13, HIGH);

    } else if (command == '0') {
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
    }
  }
}
