#include <Stepper.h>

//Pins
int inOne = 13;
int inTwo = 12;
int inThree = 14; 
int inFour = 27;
int pinCLK = 18;
int pinDT = 19;
int pinSW = 15;
int pinLimitSwitch = 5;

//Tracking Variables
byte encoderStateBuffer = 0;
int distanceFromZero = 0;

//Stepper Motor Setup
const int STEPS_PER_REV = 2038;
Stepper myStepper(STEPS_PER_REV, inOne, inThree, inTwo, inFour);


void rotaryTurned() {
  int pinCLKVal = digitalRead(pinCLK);
  int pinDTVal = digitalRead(pinDT);

  //Logic For When Rotary Encoder is Turned
  if(!(pinCLKVal == 1 && pinDTVal == 1) && !(pinCLKVal == 0 && pinDTVal == 0)) {
    //Logic For When a Clockwise Turn is Made
    if(distanceFromZero >= 0 && distanceFromZero < 20) {
      if(pinCLKVal == 0 && pinDTVal == 1){
        myStepper.setSpeed(15);
        myStepper.step(-100);
        distanceFromZero++;
      }
    }
    //Logic For When a Counter-Clockwise Turn is Made
    if(distanceFromZero > 0 && distanceFromZero <= 20) {
      if(pinDTVal == 0 && pinCLKVal == 1){
        myStepper.setSpeed(15);
        myStepper.step(100);
        distanceFromZero--;
      }
    }
  }
}

void setup() {
  //Pin Setups
  pinMode(inOne, OUTPUT);
  pinMode(inTwo, OUTPUT);
  pinMode(inThree, OUTPUT);
  pinMode(inFour, OUTPUT);
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinLimitSwitch, INPUT_PULLUP);

  delay(2000);
  myStepper.setSpeed(10);
  while(digitalRead(pinLimitSwitch) == LOW) {
    myStepper.step(-1);
  }
  distanceFromZero = 0;
}

void loop() {
  rotaryTurned();
}



