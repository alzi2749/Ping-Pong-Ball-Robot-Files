const int pinRight = 10;
const int pinLeft = 9;
const int FWD_left = 7;
const int  BACK_left = 8;
const int FWD_right = 11;
const int BACK_right = 12;

void setup() {



}


// the loop() function runs over and over again forever
void loop() {
  int VrefL = 1;
  int VrefR = VrefL;
  analogWrite(pinRight, VrefR*51);
  analogWrite(pinLeft, VrefL*51);
  digitalWrite(FWD_right, HIGH);
  digitalWrite(BACK_right, LOW);
  digitalWrite(FWD_left, HIGH);
  digitalWrite(BACK_left, LOW);
  
}