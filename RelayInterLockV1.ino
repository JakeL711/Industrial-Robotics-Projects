//Pins
int relayIn = 15;
int echo = 4;
int trig = 2;

float ultraSonicSensor(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  float duration = pulseIn(echo, HIGH);
  float distance = duration * 0.01715;

  delay(30);
  return distance;
}

void setup() {  
  Serial.begin(9600);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(relayIn, OUTPUT);

  digitalWrite(relayIn, LOW);
  delay(3000);
}

void loop() {
  float currentDistance = ultraSonicSensor();
  Serial.println(currentDistance);
  if(currentDistance <= 30){
    while(1 == 1){
      digitalWrite(relayIn, HIGH);
      delay(1000);
    }
  }
}
