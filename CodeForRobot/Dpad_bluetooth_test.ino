// -----------------------------------------------------------------
// THIS CODE IS FOR THE "DABBLE" APP (GAMEPAD MODULE)
// --- D-Pad Motor Control ---
// -----------------------------------------------------------------

#include <SoftwareSerial.h>

// Tell the Dabble library which modules we are using
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#include <Dabble.h>

// --- Your Motor Pin Definitions ---
const int pinRight = 10;    // Right motor PWM speed (Enable)
const int pinLeft = 9;      // Left motor PWM speed (Enable)
const int FWD_left = 7;     // Left motor Forward
const int BACK_left = 8;    // Left motor Backward
const int FWD_right = 11;   // Right motor Forward
const int BACK_right = 12;  // Right motor Backward

// --- Your Bluetooth Pin Definitions ---
// NOTE: This uses D2 for RX, D3 for TX
// Make sure your wiring matches!
SoftwareSerial BTSerial(2, 3); // RX, TX

// --- Your Speed Setting ---
// "Medium speed" (0 = off, 255 = full speed)
const int MEDIUM_SPEED = 150; 

void setup() {
  // Start the serial for the computer (for debugging)
  Serial.begin(9600);
  Serial.println("Dabble Robot Controller Initialized!");
  Serial.println("Press D-Pad buttons in the app.");

  // Start the serial for the HM-10 module
  BTSerial.begin(9600); 
  
  // Give the Dabble library our BTSerial port to use
  Dabble.begin(BTSerial); 

  // Set all motor pins to OUTPUT
  pinMode(pinRight, OUTPUT);
  pinMode(pinLeft, OUTPUT);
  pinMode(FWD_left, OUTPUT);
  pinMode(BACK_left, OUTPUT);
  pinMode(FWD_right, OUTPUT);
  pinMode(BACK_right, OUTPUT);

  // Start with motors stopped
  robot_stop();
}

void loop() {
  // This function reads data from the HM-10
  // and updates the Gamepad button states.
  Dabble.processInput(); 

  // --- Check for D-Pad Button Data ---
  if (GamePad.isUpPressed()) {
    Serial.println("D-PAD --- UP (Forward)");
    robot_forward();
  } 
  else if (GamePad.isDownPressed()) {
    Serial.println("D-PAD --- DOWN (Backward)");
    robot_backward();
  }
  else if (GamePad.isLeftPressed()) {
    Serial.println("D-PAD --- LEFT (Pivot Left)");
    robot_pivot_left();
  }
  else if (GamePad.isRightPressed()) {
    Serial.println("D-PAD --- RIGHT (Pivot Right)");
    robot_pivot_right();
  }
  else {
    // If no D-Pad button is pressed, stop the robot
    robot_stop();
  }
}

// --- Motor Control Functions ---

void robot_forward() {
  // Set direction
  digitalWrite(FWD_left, HIGH);
  digitalWrite(BACK_left, LOW);
  digitalWrite(FWD_right, HIGH);
  digitalWrite(BACK_right, LOW);
  // Set speed
  analogWrite(pinLeft, MEDIUM_SPEED);
  analogWrite(pinRight, MEDIUM_SPEED);
}

void robot_backward() {
  // Set direction
  digitalWrite(FWD_left, LOW);
  digitalWrite(BACK_left, HIGH);
  digitalWrite(FWD_right, LOW);
  digitalWrite(BACK_right, HIGH);
  // Set speed
  analogWrite(pinLeft, MEDIUM_SPEED);
  analogWrite(pinRight, MEDIUM_SPEED);
}

void robot_pivot_left() {
  // Set direction (Left motor back, Right motor fwd)
  digitalWrite(FWD_left, LOW);
  digitalWrite(BACK_left, HIGH);
  digitalWrite(FWD_right, HIGH);
  digitalWrite(BACK_right, LOW);
  // Set speed
  analogWrite(pinLeft, MEDIUM_SPEED);
  analogWrite(pinRight, MEDIUM_SPEED);
}

void robot_pivot_right() {
  // Set direction (Left motor fwd, Right motor back)
  digitalWrite(FWD_left, HIGH);
  digitalWrite(BACK_left, LOW);
  digitalWrite(FWD_right, LOW);
  digitalWrite(BACK_right, HIGH);
  // Set speed
  analogWrite(pinLeft, MEDIUM_SPEED);
  analogWrite(pinRight, MEDIUM_SPEED);
}

void robot_stop() {
  // Set speed to 0
  analogWrite(pinLeft, 0);
  analogWrite(pinRight, 0);
  // (Direction doesn't matter when speed is 0)
}