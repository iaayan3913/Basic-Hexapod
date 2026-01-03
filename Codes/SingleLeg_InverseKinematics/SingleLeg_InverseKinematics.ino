/*
  Single Leg Inverse Kinematics (IK) for 2-DOF Hexapod
  
  This sketch calculates the servo angles for a 2-servo leg (Hip + Knee).
  Since there is no Tibia, the leg points directly at the target vector.
  
  Coordinate System:
  X: Forward/Backward
  Y: Left/Right
  Z: Up/Down
*/

#include <Servo.h>

// --- CONFIGURATION ---
const bool CALIBRATION_MODE = false; 

// --- Physical Dimensions (Measure these on your robot in mm) ---
const float L_COXA = 29.828+29.828;   // Distance from Hip pivot to Knee pivot
const float L_LEG = 60;   // Distance from Knee pivot to Foot tip (Total length)

// --- Pin Definitions ---
const int PIN_COXA = 5;
const int PIN_KNEE = 6;

Servo coxaServo;
Servo kneeServo; 

// --- Helper Maths ---
const float RAD_TO_DEGs = 180.0 / 3.142;

// --- Gait Parameters ---
const float Z_HEIGHT = -60.0; // Height of foot when on the ground
const float Z_LIFT = 10.0;   // Height of foot when lifting
const float Y_START = -30.0;  // Start of step (Back)
const float Y_END = 100.0;     // End of step (Forward)
const float X_DIST = 90.0;   // Fixed distance from body

void setup() {
  Serial.begin(9600);
  
  coxaServo.attach(PIN_COXA);
  kneeServo.attach(PIN_KNEE);
  
  if (CALIBRATION_MODE) {
    coxaServo.write(90);
    kneeServo.write(90);
  } else {
    // Start at "Home" position
    moveToPoint(X_DIST, 0, Z_HEIGHT); 
    delay(1000);
  }
}

void loop() {
  if (CALIBRATION_MODE) return;

  // --- PHASE 1: LIFT (Up) ---
  for (float z = Z_HEIGHT; z <= Z_LIFT; z += 2) {
    moveToPoint(X_DIST, Y_START, z);
    delay(10);
  }

  // --- PHASE 2: SWING (Forward in air) ---
  for (float y = Y_START; y <= Y_END; y += 2) {
    moveToPoint(X_DIST, y, Z_LIFT);
    delay(10);
  }

  // --- PHASE 3: DROP (Down) ---
  for (float z = Z_LIFT; z >= Z_HEIGHT; z -= 2) {
    moveToPoint(X_DIST, Y_END, z);
    delay(10);
  }

  // --- PHASE 4: DRAG (Backward on ground) ---
  // This is the "power stroke" that pushes the body forward
  for (float y = Y_END; y >= Y_START; y -= 2) {
    moveToPoint(X_DIST, y, Z_HEIGHT);
    delay(10);
  }
}

/*
  Calculates IK for 2-DOF and writes to servos.
  For 2-DOF, this function aligns the leg vector with the target point.
*/
void moveToPoint(float x, float y, float z) {
  
  // --- Step 1: Compute Coxa Angle (Hip/Yaw) ---
  float theta1 = atan2(y, x) * RAD_TO_DEGs; 

  // --- Step 2: Compute Knee Angle (Pitch) ---
  // Total horizontal distance from origin
  float d_total = sqrt(sq(x) + sq(y));
  
  // Subtract Coxa to get the distance the Leg must cover horizontally
  float x_plane = d_total - L_COXA;
  float y_plane = z; 

  // Calculate the angle to point directly at the target (x_plane, z)
  // atan2(vertical, horizontal) gives the angle relative to the horizon
  float theta2 = atan2(y_plane, x_plane) * RAD_TO_DEGs;

  // --- Step 3: Mapping to Servo Values ---
  // Coxa: 90 is center
  int coxaAngle = 90 + theta1; 
  
  // Knee: 90 is Horizontal
  // If target is down (z negative), theta2 is negative.
  // We add theta2 to 90. So -45 degrees becomes servo position 45.
  // Adjust the sign (+/-) if your servo moves the wrong way!
  int kneeAngle = 90 + theta2; 

  // Constrain angles
  coxaAngle = constrain(coxaAngle, 0, 180);
  kneeAngle = constrain(kneeAngle, 0, 180);

  coxaServo.write(coxaAngle);
  kneeServo.write(kneeAngle);
}