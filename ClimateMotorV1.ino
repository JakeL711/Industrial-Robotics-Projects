//DHT Sensor Setup
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//LCD Screen Setup
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Variables
float h;
float t;
int pinMotor = 15;

void lcdScreen() {
  lcd.clear();
  //Prints Humidity Data
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.setCursor(10,0);
  lcd.print(h);
  //Prints Temp Data
  lcd.setCursor(0, 1);
  lcd.print("Temp (F): ");
  lcd.setCursor(10, 1);
  lcd.print(t);
}
void turnOnMotor() {
  if(h >= 80 || t >= 72) {
    digitalWrite(pinMotor, HIGH);
  }
  else{
    digitalWrite(pinMotor, LOW);
  }
}

void setup() {
  //DHT Start up
  dht.begin();
  //LCD Screen Start Up
  lcd.init();
  lcd.backlight();
  //Pin Setup
  pinMode(pinMotor, OUTPUT);
}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature(true);
  
  lcdScreen();
  turnOnMotor();
  delay(2000);
}





