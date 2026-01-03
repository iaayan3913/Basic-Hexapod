#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- Calibrated Settings ---
#define SERVOMIN  110 
#define SERVOMAX  550 

// --- Posture Settings ---
#define KNEE_STAND     25   // Standing height
#define KNEE_SLEEP     90   // Fully tucked/flat
#define KNEE_PUSHUP_DOWN 80 // Low position for pushups
#define HIP_CENTER     25   // Neutral stance

// --- Speed Settings ---
#define SLOW_SPEED     15
#define FAST_SPEED     5

// --- The Servo Joint Class ---
class ServoJoint {
  private:
    int channel;    
    bool isInverse; 
    String name;
    int currentAngle; // Robot remembers where it is

  public:
    ServoJoint(String n, int ch, bool inverse) {
      name = n;
      channel = ch;
      isInverse = inverse;
      currentAngle = 90; // Default startup assumption
    }

    void setAngle(int angle) {
      currentAngle = angle;
      int finalAngle = angle;

      // Inverse Logic
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
};

// --- Create the 8 Joints ---
// HIPS
ServoJoint FL_Hip("FL_Hip", 0, true);   
ServoJoint FR_Hip("FR_Hip", 1, false);  
ServoJoint BL_Hip("BL_Hip", 2, false);  
ServoJoint BR_Hip("BR_Hip", 3, true);   

// KNEES
ServoJoint FL_Knee("FL_Knee", 4, true); 
ServoJoint FR_Knee("FR_Knee", 5, true);  
ServoJoint BL_Knee("BL_Knee", 6, true); 
ServoJoint BR_Knee("BR_Knee", 7, true);  

// --- HELPER DECLARATIONS ---
void smoothMove(ServoJoint &joint, int targetAngle, int speedDelay);
void sleep();
void wake();
void wave();
void pushUps();
void sit();
void bow();

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(10);

  // 1. Start in Sleep Mode (Lying down)
  Serial.println("Starting in Sleep Mode...");
  sleep();
  delay(1000);

  // 2. Wake Up
  Serial.println("Waking Up...");
  wake();
  delay(1000);

  // 3. Push Ups
  Serial.println("Push Ups...");
  pushUps();
  delay(1000);

  // 4. Sit
  Serial.println("Sitting...");
  sit();
  delay(2000);
  wake(); // Stand back up
  delay(1000);

  // 5. Bow
  Serial.println("Bowing...");
  bow();
  delay(2000);
  wake(); // Stand back up
  delay(1000);

  // 6. Wave Hello (Your Modified Version)
  Serial.println("Waving...");
  wave();
  delay(1000);
  
  // 7. Go back to sleep
  Serial.println("Going to sleep...");
  sleep();
  delay(1000);

  Serial.println("Going back to initial position...");
  Initial();
}

void loop() {
  // Main loop is empty because we ran the demo in setup()
}

// --- HELPER: Smooth Move ---
// Moves from the CURRENT angle to the TARGET angle
void smoothMove(ServoJoint &joint, int targetAngle, int speedDelay) {
  int startAngle = joint.getAngle();
  
  if (startAngle < targetAngle) {
    for (int i = startAngle; i <= targetAngle; i++) {
      joint.setAngle(i);
      delay(speedDelay);
    }
  } else {
    for (int i = startAngle; i >= targetAngle; i--) {
      joint.setAngle(i);
      delay(speedDelay);
    }
  }
}

// --- ANIMATION: Sleep ---
// Lays flat on the floor
void sleep() {
  // 1. Move Hips to Center first
  smoothMove(FL_Hip, HIP_CENTER, SLOW_SPEED);
  smoothMove(FR_Hip, HIP_CENTER, SLOW_SPEED);
  smoothMove(BL_Hip, HIP_CENTER, SLOW_SPEED);
  smoothMove(BR_Hip, HIP_CENTER, SLOW_SPEED);
  
  // 2. Fold Knees (Tuck in to 90)
  smoothMove(FL_Knee, KNEE_SLEEP, SLOW_SPEED);
  smoothMove(FR_Knee, KNEE_SLEEP, SLOW_SPEED);
  smoothMove(BL_Knee, KNEE_SLEEP, SLOW_SPEED);
  smoothMove(BR_Knee, KNEE_SLEEP, SLOW_SPEED);
}

// --- ANIMATION: Wake ---
// Stands up tall
void wake() {
  // 1. Set Hips to Center
  FL_Hip.setAngle(HIP_CENTER);
  FR_Hip.setAngle(HIP_CENTER);
  BL_Hip.setAngle(HIP_CENTER);
  BR_Hip.setAngle(HIP_CENTER);
  
  // 2. Lift Knees to Standing Height (25)
  smoothMove(FL_Knee, KNEE_STAND, SLOW_SPEED);
  smoothMove(FR_Knee, KNEE_STAND, SLOW_SPEED);
  smoothMove(BL_Knee, KNEE_STAND, SLOW_SPEED);
  smoothMove(BR_Knee, KNEE_STAND, SLOW_SPEED);
}

// --- ANIMATION: Push Ups ---
void pushUps() {
  for(int i=0; i<3; i++) {
    // Down (knees bend to 80)
    smoothMove(FL_Knee, KNEE_PUSHUP_DOWN, FAST_SPEED);
    smoothMove(FR_Knee, KNEE_PUSHUP_DOWN, FAST_SPEED);
    smoothMove(BL_Knee, KNEE_PUSHUP_DOWN, FAST_SPEED);
    smoothMove(BR_Knee, KNEE_PUSHUP_DOWN, FAST_SPEED);
    delay(200);
    // Up (knees straighten to 25)
    smoothMove(FL_Knee, KNEE_STAND, FAST_SPEED);
    smoothMove(FR_Knee, KNEE_STAND, FAST_SPEED);
    smoothMove(BL_Knee, KNEE_STAND, FAST_SPEED);
    smoothMove(BR_Knee, KNEE_STAND, FAST_SPEED);
    delay(200);
  }
}

// --- ANIMATION: Sit ---
// Back legs down, Front legs up
void sit() {
  // Lower Back Legs
  smoothMove(BL_Knee, KNEE_SLEEP, SLOW_SPEED);
  smoothMove(BR_Knee, KNEE_SLEEP, SLOW_SPEED);
  // Ensure Front Legs are Standing (in case we weren't already)
  smoothMove(FL_Knee, KNEE_STAND, SLOW_SPEED);
  smoothMove(FR_Knee, KNEE_STAND, SLOW_SPEED);
}

// --- ANIMATION: Bow ---
// Front legs down, Back legs up
void bow() {
  // Lower Front Legs
  smoothMove(FL_Knee, KNEE_SLEEP, SLOW_SPEED);
  smoothMove(FR_Knee, KNEE_SLEEP, SLOW_SPEED);
  // Ensure Back Legs are Standing
  smoothMove(BL_Knee, KNEE_STAND, SLOW_SPEED);
  smoothMove(BR_Knee, KNEE_STAND, SLOW_SPEED);
}

// --- ANIMATION: Wave (User Modified) ---
void wave() {
  // 1. Counterbalance: Move Front Right Leg forward to stabilize
  smoothMove(FR_Hip, 60, SLOW_SPEED);
  delay(200);

  // 2. Lift FL Leg high (Knee goes to 180)
  smoothMove(FL_Knee, 180, SLOW_SPEED); 
  
  // 3. Wiggle FL Knee
  for (int i = 0; i < 3; i++) {
    smoothMove(FL_Knee, 180, FAST_SPEED); // Wave Out
    smoothMove(FL_Knee, 90, FAST_SPEED);  // Wave In
  }
  delay(500);

  // 4. Wiggle FL Hip
  FL_Knee.setAngle(170);
  for (int i = 0; i < 3; i++) {
    smoothMove(FL_Hip, 80, FAST_SPEED); // Wave Out
    smoothMove(FL_Hip, 20, FAST_SPEED); // Wave In
  }
  
  // 5. Return to Center
  smoothMove(FL_Hip, HIP_CENTER, SLOW_SPEED);
  
  // 6. Put Foot Down
  smoothMove(FL_Knee, KNEE_STAND, SLOW_SPEED);
  
  // 7. Reset Counterbalance Leg
  smoothMove(FR_Hip, HIP_CENTER, SLOW_SPEED);
}

void Initial() { 
  smoothMove(FL_Hip, 90, SLOW_SPEED);
  smoothMove(FR_Hip, 90, SLOW_SPEED);
  smoothMove(BL_Hip, 90, SLOW_SPEED);
  smoothMove(BR_Hip, 90, SLOW_SPEED);

  smoothMove(FL_Knee, 25, SLOW_SPEED);
  smoothMove(FR_Knee, 25, SLOW_SPEED);
  smoothMove(BL_Knee, 25, SLOW_SPEED);
  smoothMove(BR_Knee, 25, SLOW_SPEED);
}