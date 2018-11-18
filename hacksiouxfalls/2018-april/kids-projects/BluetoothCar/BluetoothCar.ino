#include <AFMotor.h>

#include <SoftwareSerial.h>
SoftwareSerial BT(14, 15); 
// creates a "virtual" serial port/UART
// connect BT module TX to D10
// connect BT module RX to D11
// connect BT Vcc to 5V, GND to GND

AF_DCMotor frontRight(1);
AF_DCMotor frontLeft(2);
AF_DCMotor rearLeft(3);
AF_DCMotor rearRight(4);

char a; // stores incoming character from other device

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  BT.begin(9600);

  BT.println("Hello from Arduino");

  // You might play with the speed, I'd keep all wheels the same speed
  frontLeft.setSpeed(200);
  frontRight.setSpeed(200);
  rearLeft.setSpeed(200);
  rearRight.setSpeed(200);
}

void loop() {
  if (BT.available())
  {
    a=(BT.read());
    Serial.println(a);
    if (a=='1')
    {
      // Turn Left Logic
      frontRight.run(FORWARD);
      rearRight.run(FORWARD);
      frontLeft.run(BACKWARD);
      rearLeft.run(BACKWARD);
      delay(500);
      frontRight.run(RELEASE);
      frontLeft.run(RELEASE);
      rearRight.run(RELEASE);
      rearLeft.run(RELEASE);
    }
    if (a=='2')
    {
      // Go Forward Logic
      frontRight.run(FORWARD);
      frontLeft.run(FORWARD);
      rearRight.run(FORWARD);
      rearLeft.run(FORWARD);
      delay(500);
      frontRight.run(RELEASE);
      frontLeft.run(RELEASE);
      rearRight.run(RELEASE);
      rearLeft.run(RELEASE);
    }
    if (a=='3')
    {
      // Turn Right Logic
      frontLeft.run(FORWARD);
      frontRight.run(BACKWARD);
      rearLeft.run(FORWARD);
      rearRight.run(BACKWARD);
      delay(500);
      frontRight.run(RELEASE);
      frontLeft.run(RELEASE);
      rearRight.run(RELEASE);
      rearLeft.run(RELEASE);
    }
    if (a=='4')
    {
      // Go Backward Logic
      frontRight.run(BACKWARD);
      frontLeft.run(BACKWARD);
      rearRight.run(BACKWARD);
      rearLeft.run(BACKWARD);
      delay(500);
      frontRight.run(RELEASE);
      frontLeft.run(RELEASE);
      rearRight.run(RELEASE);
      rearLeft.run(RELEASE);
    }
    // Add more "if (a == 'XX')" blocks to create more controls
    if (a=='?')
    {
      
    }   
  }

}
