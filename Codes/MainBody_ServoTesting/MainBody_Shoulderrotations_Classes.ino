#include <Servo.h>

Servo myservo;
const int servopin = 6;
const int angle_offset=0;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(servopin);
  myservo.write(120);
  // 105 is like the 90 of this, 15 deg offset
}

void loop() {
  // put your main code here, to run repeatedly:
  //testServo();


}
void testServo (){
  myservo.write(0);
  delay(1000);
  myservo.write(90);
  delay(1000);
  myservo.write(180);
  delay(1000);
  for( int i =180; i >= 0 ; i-=18){
    myservo.write(i);
    delay(100);
  }
}
