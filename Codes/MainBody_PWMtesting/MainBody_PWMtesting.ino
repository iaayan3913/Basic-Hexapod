/*
  Quadruped Full Robot Interactive Tester (8 Servos)
  
  This sketch allows you to control all 8 servos (Hips + Knees) 
  individually via the Serial Monitor for calibration.
  
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

    // Direct Pulse Control (For Calibration)
    void setPulse(int pulse) {
       pwm.setPWM(channel, 0, pulse);
       Serial.print("Joint: "); Serial.print(name);
       Serial.print(" | Ch: "); Serial.print(channel);
       Serial.print(" | RAW PULSE: "); Serial.println(pulse);
    }

    // Set angle (0 to 180)
    void setAngle(int angle) {
      int finalAngle = angle;

      // Inverse Logic for mirrored joints
      if (isInverse) {
        finalAngle = 180 - angle;
      }
      
      finalAngle = constrain(finalAngle, 0, 180);
      
      // Map Angle to Calibrated Pulse Widths
      uint16_t pulseLength = map(finalAngle, 0, 180, SERVOMIN, SERVOMAX);
      
      pwm.setPWM(channel, 0, pulseLength);

      // Print Debug Info
      Serial.print("Joint: "); Serial.print(name);
      Serial.print(" | Ch: "); Serial.print(channel);
      Serial.print(" | Input: "); Serial.print(angle);
      Serial.print(" deg | Final: "); Serial.print(finalAngle);
      Serial.print(" deg | Pulse: "); Serial.println(pulseLength);
    }
};

// --- Create the 8 Joints ---

// SHOULDERS (Channels 0-3)
ServoJoint FL_Hip("FL_Hip", 0, true);   
ServoJoint FR_Hip("FR_Hip", 1, false);  
ServoJoint BL_Hip("BL_Hip", 2, false);  
ServoJoint BR_Hip("BR_Hip", 3, true);   

// KNEES (Channels 4-7)
ServoJoint FL_Knee("FL_Knee", 4, true); 
ServoJoint FR_Knee("FR_Knee", 5, true);  
ServoJoint BL_Knee("BL_Knee", 6, true); 
ServoJoint BR_Knee("BR_Knee", 7, true);  

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing PCA9685...");
  Serial.println("---------------------------------------");
  Serial.println("COMMAND MODE (8 SERVOS):");
  Serial.println("1. ANGLE MODE: Type '[Channel] [Angle]' (0-180)");
  Serial.println("   Example: '4 90' moves FL Knee to 90 degrees.");
  Serial.println("");
  Serial.println("2. RAW MODE: Type '[Channel] [Pulse]' (>180)");
  Serial.println("   Example: '4 300' sends raw pulse width 300.");
  Serial.println("");
  Serial.println("CHANNEL MAP:");
  Serial.println("0: FL Hip   | 4: FL Knee");
  Serial.println("1: FR Hip   | 5: FR Knee");
  Serial.println("2: BL Hip   | 6: BL Knee");
  Serial.println("3: BR Hip   | 7: BR Knee");
  Serial.println("---------------------------------------");

  pwm.begin();
  pwm.setPWMFreq(50); 
  
  delay(10);

  // Center all servos immediately
  FL_Hip.setAngle(90);
  FR_Hip.setAngle(90);
  BL_Hip.setAngle(90);
  BR_Hip.setAngle(90);
  FL_Knee.setAngle(90);
  FR_Knee.setAngle(90);
  BL_Knee.setAngle(90);
  BR_Knee.setAngle(90);
}

void loop() {
  // Check if user typed something in Serial Monitor
  if (Serial.available() > 0) {
    // Read the Channel (first number)
    int ch = Serial.parseInt();
    // Read the Value (second number)
    int val = Serial.parseInt();

    // Clear the rest of the buffer (newline characters)
    while (Serial.available()) { Serial.read(); }

    Serial.print(">> Command Received: Ch "); 
    Serial.print(ch); 
    Serial.print(" -> "); 
    Serial.println(val);

    // If value is > 180, assume user wants RAW PULSE mode
    bool rawMode = (val > 180);

    // Route to the correct joint
    switch (ch) {
      case 0: if(rawMode) FL_Hip.setPulse(val);  else FL_Hip.setAngle(val);  break;
      case 1: if(rawMode) FR_Hip.setPulse(val);  else FR_Hip.setAngle(val);  break;
      case 2: if(rawMode) BL_Hip.setPulse(val);  else BL_Hip.setAngle(val);  break;
      case 3: if(rawMode) BR_Hip.setPulse(val);  else BR_Hip.setAngle(val);  break;
      
      case 4: if(rawMode) FL_Knee.setPulse(val); else FL_Knee.setAngle(val); break;
      case 5: if(rawMode) FR_Knee.setPulse(val); else FR_Knee.setAngle(val); break;
      case 6: if(rawMode) BL_Knee.setPulse(val); else BL_Knee.setAngle(val); break;
      case 7: if(rawMode) BR_Knee.setPulse(val); else BR_Knee.setAngle(val); break;
      
      default: Serial.println("Error: Invalid Channel. Use 0-7."); break;
    }
  }
}