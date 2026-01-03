/*
  Simple Servo Control
  
  This sketch sweeps a servo motor connected to pin 5 back and forth 
  from 0 to 180 degrees.
  
  Circuit:
  * Servo Red wire -> 5V
  * Servo Black/Brown wire -> GND
  * Servo Signal (Orange/Yellow) -> Pin 5
*/

#include <Servo.h>

Servo myServo;  // create servo object to control a servo
Servo myServo2;

// Define the pin where the servo is connected
const int servoPin = 5;
const int servoPin2 = 6;

void setup() {
  // attaches the servo on pin 5 to the servo object
  myServo.attach(servoPin);
  myServo2.attach(servoPin2);
  myServo.write(0);
  myServo2.write(0);
}

void loop() {
  myServo.write(0);
  myServo2.write(130);
  Serial.print("0 degress(initial position)");
  delay(1000);

  myServo.write(45);
  myServo2.write(0);
  Serial.print("45 degress");
  delay(1000);

  for (int i = 0 ; i <= 50 ; i+=2){
    myServo2.write(i);
    delay(8);
  }
  delay(50);
  for (int i = 50; i >= 0 ; i-=2 ){
    myServo2.write(i);
    delay(8);
  }
  delay(50);
  for (int i = 0 ; i <= 50 ; i+=2){
    myServo2.write(i);
    delay(8);
  }
  delay(50);
  for (int i = 50; i >= 0 ; i-=2 ){
    myServo2.write(i);
    delay(8);
  }
  delay(2000);




}