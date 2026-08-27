//Pins Sensors
int pinLeftSensor = 35;
int pinRightSensor = 34;
int pinTrig = 5;  
int pinEcho = 18;
//Pins Motor Left
int pinLeftAIN1 = 13;
int pinLeftAIN2 = 12;
int pinLeftBIN1 = 14;
int pinLeftBIN2 = 27;
//Pins Motor Right
int pinRightAIN1 = 22;
int pinRightAIN2 = 21;
int pinRightBIN1 = 23;
int pinRightBIN2 = 19;
//Speed Pins
int pinLeftPWMA = 25;
int pinLeftPWMB = 26;
int pinRightPWMA = 32;
int pinRightPWMB = 33;
//Speed
int speed = 150;
int turnSpeed = 150;

//Movement Functions
void moveFoward() {
  //Left Side
  digitalWrite(pinLeftAIN1, HIGH);
  digitalWrite(pinLeftAIN2, LOW);
  digitalWrite(pinLeftBIN1, HIGH);
  digitalWrite(pinLeftBIN2, LOW);
  analogWrite(pinLeftPWMA, speed);
  analogWrite(pinLeftPWMB, speed);

  //Right Side
  digitalWrite(pinRightAIN1, HIGH);
  digitalWrite(pinRightAIN2, LOW);
  digitalWrite(pinRightBIN1, HIGH);
  digitalWrite(pinRightBIN2, LOW);
  analogWrite(pinRightPWMA, speed);
  analogWrite(pinRightPWMB, speed);
}
void moveBackward() {
  //Left Side
  digitalWrite(pinLeftAIN1, LOW);
  digitalWrite(pinLeftAIN2, HIGH);
  digitalWrite(pinLeftBIN1, LOW);
  digitalWrite(pinLeftBIN2, HIGH);
  analogWrite(pinLeftPWMA, speed);
  analogWrite(pinLeftPWMB, speed);

  //Right Side
  digitalWrite(pinRightAIN1, LOW);
  digitalWrite(pinRightAIN2, HIGH);
  digitalWrite(pinRightBIN1, LOW);
  digitalWrite(pinRightBIN2, HIGH);
  analogWrite(pinRightPWMA, speed);
  analogWrite(pinRightPWMB, speed);
}
void turnRight() {
  //Left Side
  digitalWrite(pinLeftAIN1, HIGH);
  digitalWrite(pinLeftAIN2, LOW);
  digitalWrite(pinLeftBIN1, HIGH);
  digitalWrite(pinLeftBIN2, LOW);
  analogWrite(pinLeftPWMA, turnSpeed);
  analogWrite(pinLeftPWMB, turnSpeed);

  //Right Side
  digitalWrite(pinRightAIN1, LOW);
  digitalWrite(pinRightAIN2, HIGH);
  digitalWrite(pinRightBIN1, LOW);
  digitalWrite(pinRightBIN2, HIGH);
  analogWrite(pinRightPWMA, turnSpeed);
  analogWrite(pinRightPWMB, turnSpeed);
}
void turnLeft() {
  //Left Side
  digitalWrite(pinLeftAIN1, LOW);
  digitalWrite(pinLeftAIN2, HIGH);
  digitalWrite(pinLeftBIN1, LOW);
  digitalWrite(pinLeftBIN2, HIGH);
  analogWrite(pinLeftPWMA, turnSpeed);
  analogWrite(pinLeftPWMB, turnSpeed);

  //Right Side
  digitalWrite(pinRightAIN1, HIGH);
  digitalWrite(pinRightAIN2, LOW);
  digitalWrite(pinRightBIN1, HIGH);
  digitalWrite(pinRightBIN2, LOW);
  analogWrite(pinRightPWMA, turnSpeed);
  analogWrite(pinRightPWMB, turnSpeed);
}
void stop() {
  //Left Side
  digitalWrite(pinLeftAIN1, HIGH);
  digitalWrite(pinLeftAIN2, HIGH);
  digitalWrite(pinLeftBIN1, HIGH);
  digitalWrite(pinLeftBIN2, HIGH);
  analogWrite(pinLeftPWMA, speed);
  analogWrite(pinLeftPWMB, speed);

  //Right Side
  digitalWrite(pinRightAIN1, HIGH);
  digitalWrite(pinRightAIN2, HIGH);
  digitalWrite(pinRightBIN1, HIGH);
  digitalWrite(pinRightBIN2, HIGH);
  analogWrite(pinRightPWMA, speed);
  analogWrite(pinRightPWMB, speed);
}
//Sensor Function
float ultraSonicSensor() {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  float duration = pulseIn(pinEcho, HIGH);
  float distance = duration * 0.01715;
  
  delay(30);
  return distance;
}


void setup() {

  //Left Side Motors
  pinMode(pinLeftAIN1, OUTPUT);
  pinMode(pinLeftAIN2, OUTPUT);
  pinMode(pinLeftBIN1, OUTPUT);
  pinMode(pinLeftBIN2, OUTPUT);
  //Pins for Speed
  pinMode(pinLeftPWMA, OUTPUT);
  pinMode(pinLeftPWMB, OUTPUT);

  //Right Side Motors
  pinMode(pinRightAIN1, OUTPUT);
  pinMode(pinRightAIN2, OUTPUT);
  pinMode(pinRightBIN1, OUTPUT);
  pinMode(pinRightBIN2, OUTPUT);
  //Pins for Speed
  pinMode(pinRightPWMA, OUTPUT);
  pinMode(pinRightPWMB, OUTPUT);

  //Sensors for Light
  pinMode(pinLeftSensor, INPUT);
  pinMode(pinRightSensor, INPUT);
  //UltraSonic Sensors
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
}

void loop() {
  bool leftSensorValue = digitalRead(pinLeftSensor);
  bool rightSensorValue = digitalRead(pinRightSensor);
  float ultraSonicValue = ultraSonicSensor();
  
  if (ultraSonicValue >= 15) {
    if ((rightSensorValue == HIGH) && (leftSensorValue == HIGH)) {
      moveFoward();
    }    
    if ((rightSensorValue == LOW) && (leftSensorValue == HIGH)) {
      turnLeft(); 
      delay(45);
    }
    if ((rightSensorValue == HIGH) && (leftSensorValue == LOW)) {
      turnRight();
      delay(45);
    }
    if ((rightSensorValue == LOW) && (leftSensorValue == LOW)) {;
      moveBackward();
      delay(20);
    } 
  } else {
    stop();
  }
}



