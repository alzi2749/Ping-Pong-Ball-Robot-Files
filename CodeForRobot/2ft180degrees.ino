const int pinRight = 10;     // Right Wheel PWM Speed
const int pinLeft = 9;      // Left Wheel PWM Speed
const int FWD_left = 7;      // Left Wheel Forward
const int BACK_left = 8;     // Left Wheel Backward
const int FWD_right = 11;     // Right Wheel Forward
const int BACK_right = 12;    // Right Wheel Backward

const int LEFT_ENCODER_PIN = 4;
const int RIGHT_ENCODER_PIN = 5;
const int ButtonInput = 6;


const int MOTOR_SPEED = 100; 
const float INCHES_PER_PULSE = 0.01;


const unsigned long PULSES_FOR_180_TURN = 1270; 

const unsigned long TARGET_PULSES_FWD = 2400; // pulses for 2ft



// --- Global Variables for ISRs ---
volatile unsigned long leftEncoderCount = 0;
volatile unsigned long rightEncoderCount = 0;

// --- Interrupt Service Routines (ISRs) ---
void isrLeft() {
  leftEncoderCount++;
}
void isrRight() {
  rightEncoderCount++;
}

// --- Helper function to reset counts ---
void resetCounters() {
  // We must disable interrupts to safely write to
  // the 32-bit volatile variables
  noInterrupts();
  leftEncoderCount = 0;
  rightEncoderCount = 0;
  interrupts();
}

// --- Helper function for Active Braking ---
void activeBrake() {
  digitalWrite(FWD_right, HIGH);
  digitalWrite(BACK_right, HIGH);
  digitalWrite(FWD_left, HIGH);
  digitalWrite(BACK_left, HIGH);
  analogWrite(pinRight, 0);
  analogWrite(pinLeft, 0);
}

// --- Reusable, "Blocking" Movement Functions ---

void goForward(unsigned long targetPulses) {
  resetCounters();
  
  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), isrLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), isrRight, RISING);

  // Set motors to forward
  analogWrite(pinRight, MOTOR_SPEED);
  analogWrite(pinLeft, MOTOR_SPEED);
  digitalWrite(FWD_right, HIGH);
  digitalWrite(BACK_right, LOW);
  digitalWrite(FWD_left, HIGH);
  digitalWrite(BACK_left, LOW);

 
  while (leftEncoderCount < targetPulses && rightEncoderCount < targetPulses) {

  }

  // Goal reached! Stop motors and counting.
  activeBrake();
  detachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN));
  detachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN));
}

void goBackward(unsigned long targetPulses) {
  resetCounters();
  
  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), isrLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), isrRight, RISING);

  // Set motors to backward
  analogWrite(pinRight, MOTOR_SPEED);
  analogWrite(pinLeft, MOTOR_SPEED);
  digitalWrite(FWD_right, LOW);
  digitalWrite(BACK_right, HIGH);
  digitalWrite(FWD_left, LOW);
  digitalWrite(BACK_left, HIGH);

  // Loop here until *both* encoders pass the target
  while (leftEncoderCount < targetPulses && rightEncoderCount < targetPulses) {
    // wait
  }

  activeBrake();
  detachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN));
  detachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN));
}

void turnRight(unsigned long targetPulses) {
  resetCounters();
  
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), isrLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), isrRight, RISING);
  // Set motors for a "point turn" (Left FWD, Right BACK)
  analogWrite(pinRight, MOTOR_SPEED);
  analogWrite(pinLeft, MOTOR_SPEED);
  digitalWrite(FWD_right, LOW);
  digitalWrite(BACK_right, HIGH);
  digitalWrite(FWD_left, HIGH);
  digitalWrite(BACK_left, LOW);

  // Loop here until *both* encoders pass the target
  while (leftEncoderCount < targetPulses && rightEncoderCount < targetPulses) {
  }

  activeBrake();
  detachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN));
  detachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN));
}


void turnLeft(unsigned long targetPulses) {
  resetCounters();
  
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), isrLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), isrRight, RISING);
  // Set motors for a "point turn" (Left FWD, Right BACK)
  analogWrite(pinRight, MOTOR_SPEED);
  analogWrite(pinLeft, MOTOR_SPEED);
  digitalWrite(FWD_right, HIGH);
  digitalWrite(BACK_right, LOW);
  digitalWrite(FWD_left, LOW);
  digitalWrite(BACK_left, HIGH);

  // Loop here until *both* encoders pass the target
  while (leftEncoderCount < targetPulses && rightEncoderCount < targetPulses) {
  }

  activeBrake();
  detachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN));
  detachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN));
}

// --- Main Setup: Runs the sequence ---
void setup() {
pinMode(ButtonInput, INPUT_PULLUP);
}

// --- Main Program Loop ---
void loop() {

  pinMode(pinRight, OUTPUT);
  pinMode(pinLeft, OUTPUT);
  pinMode(FWD_left, OUTPUT);
  pinMode(BACK_left, OUTPUT);
  pinMode(FWD_right, OUTPUT);
  pinMode(BACK_right, OUTPUT);

  if(digitalRead(ButtonInput) == LOW)
  {


  // Set encoder pins as inputs
  pinMode(LEFT_ENCODER_PIN, INPUT);
  pinMode(RIGHT_ENCODER_PIN, INPUT);

  // --- Run The Sequence ---
  
  // Go forward
  goForward(TARGET_PULSES_FWD);
  delay(1000); // Pause for 1 second

  //Turn 180
  turnRight(PULSES_FOR_180_TURN);
  delay(1000); // Pause for 1 second

  //  Go back 2 feet
  goForward(TARGET_PULSES_FWD);
  delay(1000); // Pause for 1 second

//Turn 180 again
  turnLeft(PULSES_FOR_180_TURN);
  delay(1000); // Pause for 1 second

  }
}