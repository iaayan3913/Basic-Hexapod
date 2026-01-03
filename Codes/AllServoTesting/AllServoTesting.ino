/*
  Quadruped Full Sweep Test (Shoulder Cap 90, Knee Full 180)
  
  This sketch automatically sweeps the robot to verify your joint limits.
  
  CONFIG:
  - Shoulders: 0 to 90 degrees
  - Knees: 0 to 180 degrees
  
  WIRING:
  - FL Hip: Ch 0 | FL Knee: Ch 4
  - FR Hip: Ch 1 | FR Knee: Ch 5
  - BL Hip: Ch 2 | BL Knee: Ch 6
  - BR Hip: Ch 3 | BR Knee: Ch 7
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- Calibrated Settings ---
#define SERVOMIN  110 
#define SERVOMAX  550 
#define SWEEP_DELAY 15 // Time in ms between steps (Lower = Faster)

// --- The Servo Joint Class ---
class ServoJoint {
  private:
    int channel;    
    bool isInverse; 
    String name;

  public:
    ServoJoint(String n, int ch, bool inverse) {
      name = n;
      channel = ch;
      isInverse = inverse;
    }

    void setAngle(int angle) {
      int finalAngle = angle;

      // Inverse Logic
      if (isInverse) {
        finalAngle = 180 - angle;
      }
      
      finalAngle = constrain(finalAngle, 0, 180);
      
      // Map Angle to Calibrated Pulse Widths
      uint16_t pulseLength = map(finalAngle, 0, 180, SERVOMIN, SERVOMAX);
      
      pwm.setPWM(channel, 0, pulseLength);
    }
};

// --- Create the 8 Joints (YOUR CONFIGURATION) ---

// SHOULDERS (Channels 0-3)
ServoJoint FL_Hip("FL_Hip", 0, true);   
ServoJoint FR_Hip("FR_Hip", 1, false);  
ServoJoint BL_Hip("BL_Hip", 2, false);  
ServoJoint BR_Hip("BR_Hip", 3, true);   

// KNEES (Channels 4-7)
// Using the 'true' settings you confirmed work for your robot
ServoJoint FL_Knee("FL_Knee", 4, true); 
ServoJoint FR_Knee("FR_Knee", 5, true);  
ServoJoint BL_Knee("BL_Knee", 6, true); 
ServoJoint BR_Knee("BR_Knee", 7, true);  

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Sweep Test...");

  pwm.begin();
  pwm.setPWMFreq(50); 
  
  delay(10);
  
  // Start at a neutral pose
  FL_Hip.setAngle(45); FR_Hip.setAngle(45); BL_Hip.setAngle(45); BR_Hip.setAngle(45);
  FL_Knee.setAngle(90); FR_Knee.setAngle(90); BL_Knee.setAngle(90); BR_Knee.setAngle(90);
  delay(1000);
}

void loop() {
  
  // --- PHASE 1: TEST SHOULDERS (0 to 90) ---
  // Knees stay at 90 (Neutral)
  Serial.println(">>> TESTING SHOULDERS (0-90)");
  
  // Up (0 -> 90)
  for (int ang = 0; ang <= 90; ang++) {
    FL_Hip.setAngle(ang); FR_Hip.setAngle(ang); BL_Hip.setAngle(ang); BR_Hip.setAngle(ang);
    delay(SWEEP_DELAY);
  }
  delay(500);
  
  // Down (90 -> 0)
  for (int ang = 90; ang >= 0; ang--) {
    FL_Hip.setAngle(ang); FR_Hip.setAngle(ang); BL_Hip.setAngle(ang); BR_Hip.setAngle(ang);
    delay(SWEEP_DELAY);
  }
  delay(1000); // Pause before next phase


  // --- PHASE 2: TEST KNEES (0 to 180) ---
  // Shoulders stay at 45 (Middle of their range)
  Serial.println(">>> TESTING KNEES (0-180)");
  
  // Shoulders to 45
  FL_Hip.setAngle(45); FR_Hip.setAngle(45); BL_Hip.setAngle(45); BR_Hip.setAngle(45);

  // Up (0 -> 180)
  for (int ang = 0; ang <= 180; ang++) {
    FL_Knee.setAngle(ang); FR_Knee.setAngle(ang); BL_Knee.setAngle(ang); BR_Knee.setAngle(ang);
    delay(SWEEP_DELAY);
  }
  delay(500);

  // Down (180 -> 0)
  for (int ang = 180; ang >= 0; ang--) {
    FL_Knee.setAngle(ang); FR_Knee.setAngle(ang); BL_Knee.setAngle(ang); BR_Knee.setAngle(ang);
    delay(SWEEP_DELAY);
  }
  delay(1000);
}