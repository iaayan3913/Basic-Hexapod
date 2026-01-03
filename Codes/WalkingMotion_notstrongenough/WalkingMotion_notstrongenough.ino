/*
    Quadruped Walking Gait - Improved Version
    
    KEY FIXES:
    - Proper state tracking for hip positions
    - Synchronized diagonal pair stepping
    - Consistent body drag timing
    - Better movement sequencing
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- Calibrated Settings ---
#define SERVOMIN  110 
#define SERVOMAX  550 

// --- Gait Settings ---
#define WALK_SPEED     10   // Delay between small movements
#define STEP_DELAY     100  // Pause between steps
#define DRAG_SPEED     30   // Slower drag to prevent slipping

// Angles (Degrees)
#define KNEE_STAND     55   
#define KNEE_LIFT      0   

// HIP ANGLES
#define HIP_CENTER     45
#define HIP_FORWARD    65
#define HIP_BACKWARD   25

// --- The Servo Joint Class ---
class ServoJoint {
  private:
    int channel;    
    bool isInverse; 
    String name;
    int currentAngle;
    int targetAngle;  // Where we want to be

  public:
    ServoJoint(String n, int ch, bool inverse) {
      name = n;
      channel = ch;
      isInverse = inverse;
      currentAngle = 90;
      targetAngle = 90;
    }

    void setAngle(int angle) {
      currentAngle = angle;
      targetAngle = angle;
      int finalAngle = angle;

      if (isInverse) {
        finalAngle = 180 - angle;
      }
      
      finalAngle = constrain(finalAngle, 0, 180);
      uint16_t pulseLength = map(finalAngle, 0, 180, SERVOMIN, SERVOMAX);
      pwm.setPWM(channel, 0, pulseLength);
    }
    
    int getAngle() {
      return currentAngle;
    }
    
    void setTarget(int angle) {
      targetAngle = angle;
    }
    
    int getTarget() {
      return targetAngle;
    }
};

// --- Create the 8 Joints ---
ServoJoint FL_Hip("FL_Hip", 0, true);   
ServoJoint FR_Hip("FR_Hip", 1, false);  
ServoJoint BL_Hip("BL_Hip", 2, false);  
ServoJoint BR_Hip("BR_Hip", 3, true);   

ServoJoint FL_Knee("FL_Knee", 4, true); 
ServoJoint FR_Knee("FR_Knee", 5, true);  
ServoJoint BL_Knee("BL_Knee", 6, true); 
ServoJoint BR_Knee("BR_Knee", 7, true);  

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Walking Gait...");

  pwm.begin();
  pwm.setPWMFreq(50); 
  delay(10);
  
  // Initial Stand Position
  Serial.println("Standing Up...");
  FL_Hip.setAngle(HIP_CENTER); FR_Hip.setAngle(HIP_CENTER); 
  BL_Hip.setAngle(HIP_CENTER); BR_Hip.setAngle(HIP_CENTER);
  
  FL_Knee.setAngle(KNEE_STAND); FR_Knee.setAngle(KNEE_STAND); 
  BL_Knee.setAngle(KNEE_STAND); BR_Knee.setAngle(KNEE_STAND);
  
  delay(2000);
  Serial.println("Starting walk cycle...");
}

void loop() {
  // IMPROVED GAIT: Diagonal pairs move together
  // This creates a stable "trot" pattern
  
  // Phase 1: Step diagonal pair FL + BR
  stepDiagonalPair(FL_Hip, FL_Knee, BR_Hip, BR_Knee);
  
  // Phase 2: Drag body forward with planted legs (FR + BL)
  dragBodyForward();
  
  // Phase 3: Step diagonal pair FR + BL
  stepDiagonalPair(FR_Hip, FR_Knee, BL_Hip, BL_Knee);
  
  // Phase 4: Drag body forward with planted legs (FL + BR)
  dragBodyForward();
}

// --- Step two legs simultaneously (diagonal pair) ---
void stepDiagonalPair(ServoJoint &hip1, ServoJoint &knee1, 
                      ServoJoint &hip2, ServoJoint &knee2) {
  
  // 1. Lift both knees
  smoothMoveParallel(knee1, knee1.getAngle(), KNEE_LIFT,
                     knee2, knee2.getAngle(), KNEE_LIFT);
  
  // 2. Swing both hips forward
  smoothMoveParallel(hip1, hip1.getAngle(), HIP_FORWARD,
                     hip2, hip2.getAngle(), HIP_FORWARD);
  
  // 3. Drop both knees (plant feet)
  smoothMoveParallel(knee1, KNEE_LIFT, KNEE_STAND,
                     knee2, KNEE_LIFT, KNEE_STAND);
  
  delay(STEP_DELAY);
}

// --- Drag body forward with planted legs ---
void dragBodyForward() {
  // Calculate how much to drag based on current positions
  int dragAmount = (HIP_FORWARD - HIP_BACKWARD) / 2;
  
  // Get current positions
  int fl_start = FL_Hip.getAngle();
  int fr_start = FR_Hip.getAngle();
  int bl_start = BL_Hip.getAngle();
  int br_start = BR_Hip.getAngle();
  
  // Drag all hips backward together
  for (int i = 0; i < dragAmount; i++) {
    FL_Hip.setAngle(fl_start - i);
    FR_Hip.setAngle(fr_start - i);
    BL_Hip.setAngle(bl_start - i);
    BR_Hip.setAngle(br_start - i);
    
    delay(DRAG_SPEED);
  }
  
  delay(STEP_DELAY / 2);
}

// --- Move two servos in parallel smoothly ---
void smoothMoveParallel(ServoJoint &servo1, int start1, int end1,
                        ServoJoint &servo2, int start2, int end2) {
  
  int steps = max(abs(end1 - start1), abs(end2 - start2));
  
  for (int i = 0; i <= steps; i++) {
    // Calculate intermediate positions
    int pos1 = map(i, 0, steps, start1, end1);
    int pos2 = map(i, 0, steps, start2, end2);
    
    servo1.setAngle(pos1);
    servo2.setAngle(pos2);
    
    delay(WALK_SPEED);
  }
}

// --- Move single servo smoothly ---
void smoothMove(ServoJoint &servo, int startAngle, int endAngle) {
  if (startAngle < endAngle) {
    for (int pos = startAngle; pos <= endAngle; pos++) {
      servo.setAngle(pos);
      delay(WALK_SPEED);
    }
  } else {
    for (int pos = startAngle; pos >= endAngle; pos--) {
      servo.setAngle(pos);
      delay(WALK_SPEED);
    }
  }
}