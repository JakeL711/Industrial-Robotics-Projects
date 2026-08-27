
//Pins Variables
int pinWaterLevel = 13;
int pinLEDYellow = 12;
int pinLEDRed = 14;
int pinENA = 27;
int pinInOne = 26;
int pinInTwo = 25;

//Sensor Output
int waterLevelData;


void motorFullSpeed() {
  if(waterLevelData < 650) {
    analogWrite(pinENA, 255);
    digitalWrite(pinInOne, LOW);
    digitalWrite(pinInTwo, HIGH); 
    digitalWrite(pinLEDYellow, LOW);
    digitalWrite(pinLEDRed, LOW);  
  }
}
void yellowLEDWarning() {
  if(waterLevelData >= 650 && waterLevelData < 1100) {
    analogWrite(pinENA, 127);
    digitalWrite(pinInOne, LOW);
    digitalWrite(pinInTwo, HIGH); 
    digitalWrite(pinLEDYellow, HIGH);
    digitalWrite(pinLEDRed, LOW);   
  }
}
void redLEDWarning() {
  if(waterLevelData >= 1100) {
    digitalWrite(pinInOne, LOW);
    digitalWrite(pinInTwo, LOW); 
    digitalWrite(pinLEDYellow, LOW);
    digitalWrite(pinLEDRed, HIGH); 
  }
}

void setup() {
  pinMode(pinWaterLevel, INPUT);
  pinMode(pinLEDYellow, OUTPUT);
  pinMode(pinLEDRed, OUTPUT);

  pinMode(pinENA, OUTPUT);
  pinMode(pinInOne, OUTPUT);
  pinMode(pinInTwo, OUTPUT);
  Serial.begin(9600);
}


//650 yellow light turns on, and that stays until 1100 were red light turns on.
void loop() {
  waterLevelData = analogRead(pinWaterLevel);
  motorFullSpeed();
  yellowLEDWarning();
  redLEDWarning();
  Serial.println(waterLevelData);
  delay(500);
}

