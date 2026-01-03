/*
  Quadruped Shoulder Sweep Test
  
  This sketch automatically sweeps all 4 shoulder servos back and forth
  using the calibrated pulse limits.
  
  Use this to visually verify that all legs move:
  1. To the full extent (0 to 180)
  2. At the same speed
  3. In the correct mirrored directions (thanks to the class logic)
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- Calibrated Settings ---
// ALWAYS set these to the physical limits of the servo (0 and 180)
// Do not change these to limit range; change the loop instead.
#define SERVOMIN  110 
#define SERVOMAX  550 

// --- Safety Limits ---
// Change these to restrict the movement range
#define SAFE_START_ANGLE 0
#define SAFE_END_ANGLE   90

// Speed of the sweep (lower is faster)
#define SWEEP_DELAY 15 

// --- The Shoulder Class ---
class Shoulder {
  private:
    int channel;    
    bool isInverse; 
    String name;

  public:
    Shoulder(String n, int ch, bool inverse) {
      name = n;
      channel = ch;
      isInverse = inverse;
    }

    void setAngle(int angle) {
      int finalAngle = angle;

      // Inverse Logic for mirrored legs
      if (isInverse) {
        finalAngle = 180 - angle;
      }
      
      finalAngle = constrain(finalAngle, 0, 180);
      
      // Map Angle to Calibrated Pulse Widths
      uint16_t pulseLength = map(finalAngle, 0, 180, SERVOMIN, SERVOMAX);
      
      pwm.setPWM(channel, 0, pulseLength);
    }
};

// --- Create the 4 Shoulders ---
// FL/BL are Normal (False), FR/BR are Inverse (True)
Shoulder FL("FL", 0, true);   
Shoulder FR("FR", 1, false);  
Shoulder BL("BL", 2, false);  
Shoulder BR("BR", 3, true);   

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Sweep Test...");

  pwm.begin();
  pwm.setPWMFreq(50); 
  
  delay(10);
}

void loop() {
  // Sweep UP from Safe Start to Safe End
  Serial.print("Sweeping UP -> "); Serial.println(SAFE_END_ANGLE);
  for (int angle = SAFE_START_ANGLE; angle <= SAFE_END_ANGLE; angle++) {
    FL.setAngle(angle);
    FR.setAngle(angle);
    BL.setAngle(angle);
    BR.setAngle(angle);
    delay(SWEEP_DELAY);
  }
  
  delay(500); // Pause at the top

  // Sweep DOWN from Safe End to Safe Start
  Serial.print("Sweeping DOWN -> "); Serial.println(SAFE_START_ANGLE);
  for (int angle = SAFE_END_ANGLE; angle >= SAFE_START_ANGLE; angle--) {
    FL.setAngle(angle);
    FR.setAngle(angle);
    BL.setAngle(angle);
    BR.setAngle(angle);
    delay(SWEEP_DELAY);
  }
  
  delay(500); // Pause at the bottom
}