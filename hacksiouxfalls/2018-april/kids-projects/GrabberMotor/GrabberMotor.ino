
#include <AFMotor.h>

int buttonLeft = A0;
int buttonRight = A1;
int buttonOpen = A2;
int buttonClose = A3;

AF_DCMotor motorRotate(1);
AF_DCMotor motorGrabber(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  pinMode(buttonOpen, INPUT);
  pinMode(buttonClose, INPUT);

  // turn on motor
  motorRotate.setSpeed(200);
  motorGrabber.setSpeed(200);
 
  motorRotate.run(RELEASE);
  motorGrabber.run(RELEASE);
}

void loop() {
 
  int buttonStateLeft = digitalRead(buttonLeft);
  int buttonStateRight = digitalRead(buttonRight);
  int buttonStateOpen = digitalRead(buttonOpen);
  int buttonStateClose = digitalRead(buttonClose);

  if (buttonStateLeft == HIGH) {
    Serial.println("Button Left");    
    motorRotate.run(BACKWARD);
    delay(250);
    motorRotate.run(RELEASE);
    
  } else if (buttonStateRight == HIGH) {
    Serial.println("Button Right");
    motorRotate.run(FORWARD);
    delay(250);
    motorRotate.run(RELEASE);
    
  } else if (buttonStateOpen == HIGH) {
    Serial.println("Button Open");
    motorGrabber.run(BACKWARD);
    delay(150);
    motorGrabber.run(RELEASE);
    
  
  } else if (buttonStateClose == HIGH) {
    Serial.println("Button Close");
    motorGrabber.run(FORWARD);
    delay(150);
    motorGrabber.run(RELEASE);
  }
}

