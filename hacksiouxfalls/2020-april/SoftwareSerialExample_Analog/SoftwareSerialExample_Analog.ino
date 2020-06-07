#include <SoftwareSerial.h>
#include <AFMotor.h>

const int rxPin = A0;
const int txPin = A1;
SoftwareSerial mySerial(rxPin, txPin); // RX, TX

AF_DCMotor mLeft(3);
AF_DCMotor mRight(2);

void setup() {
  Serial.begin(9600);
  Serial.println("Goodnight moon!");

  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);

  mLeft.setSpeed(255);
  mRight.setSpeed(255);
  mLeft.run(FORWARD);
  mRight.run(FORWARD);
  delay(1000);
  mLeft.run(RELEASE);
  mRight.run(RELEASE);

}

void loop() {
  if (mySerial.available()) {
    char command = mySerial.read();

    Serial.print("cmd: ");
    Serial.println(command);

    if (command == 'f' || command == 'F') {
      Serial.println("forward");
      mLeft.setSpeed(255);
      mRight.setSpeed(255);
      mLeft.run(FORWARD);
      mRight.run(FORWARD);
      
    } else if (command == 'b' || command == 'B') {
      Serial.println("backward");
      mLeft.setSpeed(255);
      mRight.setSpeed(255);
      mLeft.run(BACKWARD);
      mRight.run(BACKWARD);

    } else if (command == 'l' || command == 'L') {
      Serial.println("left");
      mRight.setSpeed(255);
      mRight.run(FORWARD);

    } else if (command == 'r' || command == 'R' ) {
      Serial.println("right");
      mLeft.setSpeed(255);
      mLeft.run(FORWARD);

    } else if (command == 'e' || command == 'E' ) {

      mRight.setSpeed(255);
      mRight.run(BACKWARD);
      mLeft.setSpeed(255);
      mLeft.run(FORWARD);
      delay(2000);
      mRight.setSpeed(255);
      mRight.run(FORWARD);
      mLeft.setSpeed(255);
      mLeft.run(BACKWARD);
      delay(2000);
      mLeft.setSpeed(0);
      mRight.setSpeed(0);

    } else if (command == 'm' || command == 'M' ) {

      mRight.setSpeed(255);
      mRight.run(FORWARD);
      mLeft.setSpeed(255);
      mLeft.run(FORWARD);
      delay(500);
      mRight.setSpeed(255);
      mRight.run(BACKWARD);
      mLeft.setSpeed(255);
      mLeft.run(BACKWARD);
      delay(500);
      mLeft.setSpeed(0);
      mRight.setSpeed(0);
      
    } else if (command == 'w' || command == 'W' ) {

      for (int i = 0; i<5; i++) {
        mRight.setSpeed(255);
        mRight.run(BACKWARD);
        mLeft.setSpeed(255);
        mLeft.run(FORWARD);
        delay(100);
        mRight.setSpeed(255);
        mRight.run(FORWARD);
        mLeft.setSpeed(255);
        mLeft.run(BACKWARD);
        delay(100);
      }
      mLeft.setSpeed(0);
      mRight.setSpeed(0);
      
      
    } else if (command == '0' || command == 'O' || command == 's' || command == 'S' ) {
      Serial.println("stop");
      // Stop
      mLeft.setSpeed(0);
      mRight.setSpeed(0);
    }
  }
  
}
