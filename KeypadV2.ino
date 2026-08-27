#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

int pinLocation = 11;
int timesTried = 1;
String correctCode = "1234";
String inputtedCode = "";
char keyPress;

int greenLED = 2;
int redLED = 4;
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);


//Code to reset the pinLocation Variable
void resetKeypad(int pinNum){
  if(keyPress == '*'){
    inputtedCode = "";
    pinLocation = pinNum;
    lcd.clear();
    lcd.print("SYSTEM LOCKED");
    lcd.setCursor(0, 1);
    lcd.print("Enter Pin: ");
  }
}
//DISPLAYS ACCESS GRANTED WHEN THE RIGHT CODE IS PRESSED
void correctPin(){
  lcd.clear();
  lcd.print("ACCESS GRANTED:");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  timesTried = 1;
}
//DISPLAYS IF THE WRONG CODE IS PRESSED AND ADDS AN ATTEMPT TO THE ATTEMPT TRACKER
void wrongPin(){
    //Shows the Access Denied and Adds an Attempt
    lcd.clear();
    lcd.print("ACCESS DENIED:");
    lcd.setCursor(0,1);
    lcd.print("Attempt: ");
    lcd.setCursor(9, 1);
    lcd.print(timesTried);
    timesTried = timesTried + 1;
            
    delay(3000);
            
    //Reset the Screen Back to Enter Pin Mode
    inputtedCode = "";
    pinLocation = 10;
    lcd.clear();
    lcd.print("SYSTEM LOCKED");
    lcd.setCursor(0, 1);
    lcd.print("Enter Pin: ");
}
//LOCKS THE KEYPAD AFTER TO MAY FAILED PASSWORDS
void lockoutFunction(){
  if(timesTried == 4){
    lcd.clear();
    lcd.print("SYSTEM LOCKED");
    lcd.setCursor(0, 1);
    lcd.print("Forever!");
    while(1 == 1){
      delay(1000);
    }
  }
}

void setup() {
  
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM LOCKED");
  lcd.setCursor(0, 1);
  lcd.print("Enter Pin: ");

}

void loop() {
  keyPress = customKeypad.getKey();

  if(keyPress != 0){
    //Prints the Characters to the Screen
    lcd.setCursor(pinLocation, 1);
    lcd.print(keyPress);
    
    //Stores the Charcters to the Input Key
    inputtedCode += keyPress;
    
    //If 4 numbers typed
    if(pinLocation == 14){
      delay(500);
      lcd.clear();
      lcd.print("Enter/Clear");
      
      while(pinLocation == 14){
        keyPress = customKeypad.getKey();
        if(keyPress == '#'){
          if(correctCode == inputtedCode){  
            correctPin();
          }
          else{
            wrongPin();
            break;
          }
        }
        resetKeypad(10);
        if(pinLocation == 10) {break;}
      }
    }
    pinLocation = pinLocation + 1;
    //Clear Section
    resetKeypad(11);     
  }

  //LockOut Section
  lockoutFunction();
}
