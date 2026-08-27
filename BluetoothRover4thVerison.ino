//Pin 2 Connects to Bluetooth TXD, Pin 3 Connects to Bluetooth RXD
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial Bluetooth (2,3); 
char commandReceived;

//Variables to Store The Raw MPU-6050 Data
int16_t rawAccX, rawAccY, rawAccZ;
int16_t rawGyroX, rawGyroY, rawGyroZ;

//Right Side Motors
int rightPWMA = 11;
int rightPWMB = 10;
int rightAIN1 = 13;
int rightAIN2 = 12;
int rightBIN1 = 8;
int rightBIN2 = 7;

//Left Side Motors
int leftPWMA = 6;
int leftPWMB = 5;
int leftAIN1 = 4;
int leftAIN2 = 9;
int leftBIN1 = A3;
int leftBIN2 = A2;

//Speeds
int fullSpeed = 127;
int lowSpeed = 63;

int clearDelay = 100;

float offsetZ = 0.18;
float offsetX = 0.06;
float offsetY = 0.01;

//Functions To Move
void moveFoward(){
  //Right Side Motor 1
  analogWrite(rightPWMA, fullSpeed);
  digitalWrite(rightAIN1, HIGH);
  digitalWrite(rightAIN2, LOW);
  //Right Side Motor 2
  analogWrite(rightPWMB, fullSpeed);
  digitalWrite(rightBIN1, HIGH);
  digitalWrite(rightBIN2, LOW);
  
  //Left Side Motor 1
  analogWrite(leftPWMA, fullSpeed);
  digitalWrite(leftAIN1, HIGH);
  digitalWrite(leftAIN2, LOW);
  //Left Side Motor 2
  analogWrite(leftPWMB, fullSpeed);
  digitalWrite(leftBIN1, HIGH);
  digitalWrite(leftBIN2, LOW);
}

void moveBackward(){
  //Right Side Motor 1
  analogWrite(rightPWMA, fullSpeed);
  digitalWrite(rightAIN1, LOW);
  digitalWrite(rightAIN2, HIGH);
  //Right Side Motor 2
  analogWrite(rightPWMB, fullSpeed);
  digitalWrite(rightBIN1, LOW);
  digitalWrite(rightBIN2, HIGH);
  
  //Left Side Motor 1
  analogWrite(leftPWMA, fullSpeed);
  digitalWrite(leftAIN1, LOW);
  digitalWrite(leftAIN2, HIGH);
  //Left Side Motor 2
  analogWrite(leftPWMB, fullSpeed);
  digitalWrite(leftBIN1, LOW);
  digitalWrite(leftBIN2, HIGH);
}

void turnRight(){
  //Right Side Motor 1
  analogWrite(rightPWMA, fullSpeed);
  digitalWrite(rightAIN1, LOW);
  digitalWrite(rightAIN2, HIGH);
  //Right Side Motor 2
  analogWrite(rightPWMB, fullSpeed);
  digitalWrite(rightBIN1, LOW);
  digitalWrite(rightBIN2, HIGH);
  
  //Left Side Motor 1
  analogWrite(leftPWMA, fullSpeed);
  digitalWrite(leftAIN1, HIGH);
  digitalWrite(leftAIN2, LOW);
  //Left Side Motor 2
  analogWrite(leftPWMB, fullSpeed);
  digitalWrite(leftBIN1, HIGH);
  digitalWrite(leftBIN2, LOW);
}

void turnLeft(){
  //Right Side Motor 1
  analogWrite(rightPWMA, fullSpeed);
  digitalWrite(rightAIN1, HIGH);
  digitalWrite(rightAIN2, LOW);
  //Right Side Motor 2
  analogWrite(rightPWMB, fullSpeed);
  digitalWrite(rightBIN1, HIGH);
  digitalWrite(rightBIN2, LOW);
  
  //Left Side Motor 1
  analogWrite(leftPWMA, fullSpeed);
  digitalWrite(leftAIN1, LOW);
  digitalWrite(leftAIN2, HIGH);
  //Left Side Motor 2
  analogWrite(leftPWMB, fullSpeed);
  digitalWrite(leftBIN1, LOW);
  digitalWrite(leftBIN2, HIGH);
}

void fullBreak(){
  //Right Side Motor 1
  analogWrite(rightPWMA, lowSpeed);
  digitalWrite(rightAIN1, HIGH);
  digitalWrite(rightAIN2, HIGH);
  //Right Side Motor 2
  analogWrite(rightPWMB, lowSpeed);
  digitalWrite(rightBIN1, HIGH);
  digitalWrite(rightBIN2, HIGH);
  
  //Left Side Motor 1
  analogWrite(leftPWMA, lowSpeed);
  digitalWrite(leftAIN1, HIGH);
  digitalWrite(leftAIN2, HIGH);
  //Left Side Motor 2
  analogWrite(leftPWMB, lowSpeed);
  digitalWrite(leftBIN1, HIGH);
  digitalWrite(leftBIN2, HIGH);
}

float accelerometerData(){
  //Starts the Accelerometer's Register Box
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);  

  //Requests 14 Consectutive Bytes of Motion Data
  Wire.requestFrom(0x68, 14, true);
  
  //Glues the broken Binary Data Pack Back Together
  rawAccX = Wire.read() <<8 | Wire.read();
  rawAccY = Wire.read() <<8 | Wire.read();
  rawAccZ = Wire.read() <<8 | Wire.read();

  int16_t rawTemp = Wire.read() <<8 | Wire.read();

  rawGyroX = Wire.read() <<8 | Wire.read();
  rawGyroY = Wire.read() <<8 | Wire.read();
  rawGyroZ = Wire.read() <<8 | Wire.read();

  //Conversions to Real Physics Quantities
  float G_ForceX = (rawAccX/16384.0) + offsetX;
  float G_ForceY = (rawAccY/16384.0) - offsetY;
  float G_ForceZ = (rawAccZ/16384.0) + offsetZ;
  float SpinZ = rawGyroZ/131;

  //Sends the MPU-6050 Data to the Bluetooth Module, That Then Sends to The Cool Term Software 
  Bluetooth.print(" Tilt_X (G): "); 
  Bluetooth.println(G_ForceX);
  Bluetooth.print(" Tilt_Y (G): "); 
  Bluetooth.println(G_ForceY);
  Bluetooth.print(" Tilt_Z (G): "); 
  Bluetooth.println(G_ForceZ);
  Bluetooth.print(" SpinZ (DEG/SEC): "); 
  Bluetooth.println(SpinZ);
  Bluetooth.println("");

}

void clearBluetooth(){
  while (Bluetooth.available()) {
    Bluetooth.read();
    delay(10);
  }
}

void setup() {
  //Bluetooth Setup
  
  Bluetooth.begin(9600);
  Wire.begin();

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();
  
  //Pins For Right Side
  pinMode(rightPWMA, OUTPUT);
  pinMode(rightPWMB, OUTPUT);
  pinMode(rightAIN1, OUTPUT);
  pinMode(rightAIN2, OUTPUT);
  pinMode(rightBIN1, OUTPUT);
  pinMode(rightBIN2, OUTPUT);

  //Pins For Left Side
  pinMode(leftPWMA, OUTPUT);
  pinMode(leftPWMB, OUTPUT);
  pinMode(leftAIN1, OUTPUT);
  pinMode(leftAIN2, OUTPUT);
  pinMode(leftBIN1, OUTPUT);
  pinMode(leftBIN2, OUTPUT);

  delay(2000);
  accelerometerData();
}

void loop() {

  if (Bluetooth.available() > 0) {
  //Start of if Key is Held
    commandReceived = 'q';
    delay(50);
    commandReceived = Bluetooth.read();
    
    //Move Foward if w is Held
    if (commandReceived == 'w'){
      commandReceived = 'q';
      delay(50);
      commandReceived = Bluetooth.read();

      if(commandReceived == 'w'){
        accelerometerData();
        moveFoward();
        accelerometerData();
        clearBluetooth();
        delay(clearDelay);
      }
    }

    //Move Foward if a is Held
    if (commandReceived == 'a'){
      commandReceived = 'q';
      delay(50);
      commandReceived = Bluetooth.read();

      if(commandReceived == 'a'){
        accelerometerData();
        turnLeft();
        accelerometerData();
        clearBluetooth();
        delay(clearDelay);
      }
    }
  
    //Move Foward if s is Held
    if (commandReceived == 's'){
      commandReceived = 'q';
      delay(50);
      commandReceived = Bluetooth.read();

      if(commandReceived == 's'){
        accelerometerData();
        moveBackward();
        accelerometerData();
        clearBluetooth();
        delay(clearDelay);
      }
    }
  
    //Move Foward if d is Held
    if (commandReceived == 'd'){
      commandReceived = 'q';
      delay(50);
      commandReceived = Bluetooth.read();

      if(commandReceived == 'd'){
        accelerometerData();
        turnRight();
        accelerometerData();
        clearBluetooth();
        delay(clearDelay);
      }
    }
  }

  if(Bluetooth.available() <= 0){
    fullBreak();
    delay(200);
  }
}





