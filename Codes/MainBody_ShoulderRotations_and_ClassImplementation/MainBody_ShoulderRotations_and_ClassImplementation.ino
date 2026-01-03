/*
  Simple Shoulder Tester
  
  This code isolates just the shoulder (Coxa) joint to verify:
  1. Pin assignments
  2. "Inverse" logic (mirroring for the right side)
  3. Servo centering
*/

#include <Servo.h>

// --- The Shoulder Class ---
class Shoulder {
  private:
    Servo servo;
    int pin;
    bool isInverse; // If true, angle becomes (180 - angle)

  public:
    // Constructor
    Shoulder(int p, bool inverse) {
      pin = p;
      isInverse = inverse;
    }

    void attach() {
      servo.attach(pin);
    }

    // Set angle directly (0 to 180, where 90 is center)
    void setAngle(int angle) {
      int finalAngle = angle;

      // Logic: If this is an inverse side, flip the angle
      // 0 becomes 180, 45 becomes 135, 90 stays 90
      if (isInverse) {
        finalAngle = 180 - angle;
      }

      servo.write(constrain(finalAngle, 0, 180));
    }
};

// --- Create the 4 Shoulders ---
// Syntax: Shoulder name(Pin, IsInverse?);

// Left Side: Standard (0 is one way, 180 is the other)
Shoulder FL(2, true); // Front Left
Shoulder BL(4, false); // Back Left

// Right Side: Inverse (mirrored movement)
Shoulder FR(6, false);  // Front Right
Shoulder BR(8, true);  // Back Right

void setup() {
  Serial.begin(9600);
  
  FL.attach();
  FR.attach();
  BL.attach();
  BR.attach();
  
  // Center all servos immediately to check mechanical alignment
  Serial.println("Moving to Center (90)...");
  FL.setAngle(60);
  FR.setAngle(60);
  BL.setAngle(60);
  BR.setAngle(60);
  
  delay(2000);
}

void loop() {
  // Simple Sweep Test
  // If your setup is correct, both sides should move "Forward" together
  // regardless of which side they are on.
  
  // Sweep "Forward" (Assuming 135 is forward)
  for (int pos = 10; pos <= 100; pos++) {
    FL.setAngle(pos);
    FR.setAngle(pos);
    BL.setAngle(pos);
    BR.setAngle(pos);
    delay(10);
  }
  
  delay(500);

  // Sweep "Backward"
  for (int pos = 100; pos >= 10; pos--) {
    FL.setAngle(pos);
    FR.setAngle(pos);
    BL.setAngle(pos);
    BR.setAngle(pos);
    delay(10);
  }
  
  delay(500);
}