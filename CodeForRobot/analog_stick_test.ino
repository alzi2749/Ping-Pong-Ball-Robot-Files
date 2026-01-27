#include <SoftwareSerial.h>

// Tell the Dabble library which modules we are using
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#include <Dabble.h>

// Using D3 for RX, D2 for TX
SoftwareSerial BTSerial(2, 3); // RX, TX

// --- Motor Pin Definitions ---
const int pinRight = 10;    // Right motor PWM speed
const int pinLeft = 9;      // Left motor PWM speed
const int FWD_left = 7;     // Left motor Forward
const int BACK_left = 8;    // Left motor Backward
const int FWD_right = 11;   // Right motor Forward
const int BACK_right = 12;  // Right motor Backward
const int steeringConst = 2;

// --- NEW: Aux Motor (Pin 6) Definitions ---
const int pinAux = 6;            // The pin for the 3rd motor
double currentAuxSpeed = 0.0;    // Tracks the current speed (0-255)
int targetAuxState = 0;          // 0 = Ramp Down/Off, 1 = Ramp Up/On
unsigned long lastRampTime = 0;  // Timer for the ramp
const int rampInterval = 27;     // Time between speed steps (7000ms / 255 steps = ~27ms)

void setup() {
  Serial.begin(9600);
  Serial.println("Dabble Robot Control Initialized!");

  BTSerial.begin(9600); 
  Dabble.begin(BTSerial);

  // --- Drive Motors ---
  pinMode(pinRight, OUTPUT);
  pinMode(pinLeft, OUTPUT);
  pinMode(FWD_left, OUTPUT);
  pinMode(BACK_left, OUTPUT);
  pinMode(FWD_right, OUTPUT);
  pinMode(BACK_right, OUTPUT);
  
  // --- NEW: Aux Motor ---
  pinMode(pinAux, OUTPUT);
  // Ensure motor starts off
  analogWrite(pinAux, 0);
}

void loop() {
  Dabble.processInput();

  // ==========================================================
  // 1. DRIVE CONTROL (Existing Logic)
  // ==========================================================
  
  float xAxis = GamePad.getXaxisData();
  float yAxis = GamePad.getYaxisData();

  // Basic deadzone to prevent drift
  if (abs(xAxis) < 0.2) xAxis = 0;
  if (abs(yAxis) < 0.2) yAxis = 0;

  int throttle = (int)map(yAxis, -7.0, 7.0, -255, 255);
  int steering = (int)map(xAxis, -7.0, 7.0, -255, 255);

  int leftSpeed = throttle + steering/steeringConst;
  int rightSpeed = throttle - steering/steeringConst;

  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  setLeftMotor(leftSpeed);
  setRightMotor(rightSpeed);

  // ==========================================================
  // 2. AUX MOTOR CONTROL (Pin 6 Ramp Logic)
  // ==========================================================

  // Check Buttons to set the "Target" state
  // Circle = Ramp Up, Cross = Ramp Down
  if (GamePad.isCirclePressed()) {
    targetAuxState = 1; 
  }
  if (GamePad.isCrossPressed()) {
    targetAuxState = 0; 
  }

  // Handle the Ramping (Runs every 27ms)
  if (millis() - lastRampTime >= rampInterval) {
    lastRampTime = millis(); // Reset timer

    if (targetAuxState == 1) {
      // RAMP UP Logic
      if (currentAuxSpeed < 255) {
        currentAuxSpeed += 1.0; 
      }
    } 
    else {
      // RAMP DOWN Logic
      if (currentAuxSpeed > 0) {
        currentAuxSpeed -= 1.0; 
      }
    }

    // Safety constrain to ensure we stay within valid PWM range
    if (currentAuxSpeed > 255) currentAuxSpeed = 255;
    if (currentAuxSpeed < 0) currentAuxSpeed = 0;

    // Apply speed to Pin 6
    analogWrite(pinAux, (int)currentAuxSpeed);
  }
}

// --- Helper Functions to control drive motors ---

void setLeftMotor(int speed) {
  if (speed > 0) { // Go Forward
    digitalWrite(FWD_left, HIGH);
    digitalWrite(BACK_left, LOW);
    analogWrite(pinLeft, speed); 
  }
  else if (speed < 0) { // Go Backward
    digitalWrite(FWD_left, LOW);
    digitalWrite(BACK_left, HIGH);
    analogWrite(pinLeft, abs(speed)); 
  }
  else { // Stop
    digitalWrite(FWD_left, LOW);
    digitalWrite(BACK_left, LOW);
    analogWrite(pinLeft, 0);
  }
}


void setRightMotor(int speed) {
  if (speed > 0) { // Go Forward
    digitalWrite(FWD_right, HIGH);
    digitalWrite(BACK_right, LOW);
    analogWrite(pinRight, speed);
  }
  else if (speed < 0) { // Go Backward
    digitalWrite(FWD_right, LOW);
    digitalWrite(BACK_right, HIGH);
    analogWrite(pinRight, abs(speed));
  }
  else { // Stop
    digitalWrite(FWD_right, LOW);
    digitalWrite(BACK_right, LOW);
    analogWrite(pinRight, 0);
  }
}