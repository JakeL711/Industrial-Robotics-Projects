#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

//Encoder Pin Variables
int pinCLK = 18;
int pinDT = 19;
int pinSW = 5;

//Tracking Variable
int lastCLKState;
int currentCLKState;
int menuCounter = 1;
int totalMenuItems = 6;
int currentPage = 1;
byte encoderStateBuffer = 0;

//Accelometer Variables
int16_t rawAccX, rawAccY, rawAccZ;
int16_t rawGyroX, rawGyroY, rawGyroZ;

float G_ForceX;
float G_ForceY;
float G_ForceZ;

float offsetX = 0.02;
float offsetY = 0.02;
float offsetZ = 0;

String mainMenuList[6] = {"1. Gyro Real-Time", "2. Cal. Sensor", "3. Display Stats", "4. Wire Count", "5. Components", "6. About Page"};

//Page 1 of the Main Menu
void mainMenuOne(){
  if (menuCounter < 4) {
    //Title of Main Page
    display.clearDisplay();
    display.setCursor(10, 0);
    display.println("SYSTEM DIAGNOSTICS:");

    //The Menu Page Items
    display.setCursor(20, 20);
    display.println(mainMenuList[0]);
    display.setCursor(20, 35);
    display.println(mainMenuList[1]);
    display.setCursor(20, 50);
    display.println(mainMenuList[2]);

    //Arrow Scroll System
    if (menuCounter == 1) {
      display.setCursor(5, 20);
      display.println(">");
    }
    if (menuCounter == 2) {
      display.setCursor(5, 35);
      display.println(">");
    }
    if (menuCounter == 3) {
      display.setCursor(5, 50);
      display.println(">");
    }
    display.display();
  }
}
//Page 2 of the Main Menu
void mainMenuTwo(){
  if (menuCounter > 3) {
    //Title of Main Page
    display.clearDisplay();
    display.setCursor(10, 0);
    display.println("SYSTEM DIAGNOSTICS:");

    //The Menu Page Items
    display.setCursor(20, 20);
    display.println(mainMenuList[3]);
    display.setCursor(20, 35);
    display.println(mainMenuList[4]);
    display.setCursor(20, 50);
    display.println(mainMenuList[5]);

    //Arrow Scroll System
    if (menuCounter == 4) {
      display.setCursor(5, 20);
      display.println(">");
    }
    if (menuCounter == 5) {
      display.setCursor(5, 35);
      display.println(">");
    }
    if (menuCounter == 6) {
      display.setCursor(5, 50);
      display.println(">");
    }
    display.display();
  }
}
//Loading Page 
void loadingPage(){
  display.clearDisplay();
  display.setCursor(10, 25);
  display.print("LOADING PAGE: "); 
  display.print(menuCounter);
  display.display();
  delay(2000);
}
//Gets the raw data from the accelometer and converts to G's
void accelometerData() {
  //Get the Data from the Accelometer
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(0x68, 14, true);

  rawAccX = Wire.read() <<8 | Wire.read();
  rawAccY = Wire.read() <<8 | Wire.read();
  rawAccZ = Wire.read() <<8 | Wire.read();

  int16_t rawTemp = Wire.read() <<8 | Wire.read();

  rawGyroX = Wire.read() <<8 | Wire.read();
  rawGyroY = Wire.read() <<8 | Wire.read();
  rawGyroZ = Wire.read() <<8 | Wire.read();

  //Converstions
  G_ForceX = (rawAccX/16384.0) + offsetX;
  G_ForceY = (rawAccY/16384.0) - offsetY;
  G_ForceZ = (rawAccZ/16384.0) - offsetZ;
}
//Page 1 When Encoder is Pressed
void accelometerPage() {
  //Title
  display.clearDisplay();
  display.setCursor(10, 5);
  display.print("Gyro Data:");
  //Description
  display.setCursor(20, 20);
  display.print("G_ForceX: ");
  display.println(G_ForceX);
  display.setCursor(20, 35);
  display.print("G_ForceY: ");
  display.println(G_ForceY);
  display.setCursor(20, 50);
  display.print("G_ForceZ: ");
  display.println(G_ForceZ);

  display.display();
}
//Page 2 When Encoder is Pressed
void calibrationPage() {
  //Title
  display.clearDisplay();
  display.setCursor(10, 5);
  display.print("Cali Data:");
  //Description
  display.setCursor(20, 20);
  display.print("X_Offset: ");
  display.println(offsetX);
  display.setCursor(20, 35);
  display.print("Y_Offset: ");
  display.println(offsetY);
  display.setCursor(20, 50);
  display.print("Z_Offset: ");
  display.println(offsetZ);
  
  display.display();
}
//Page 3 When Encoder is Pressed
void displayStats() {
  display.clearDisplay();
  display.setCursor(10, 5);
  display.print("Display Stats:");

  display.setCursor(20, 20);
  display.println("X_Size: 128");
  display.setCursor(20, 35);
  display.println("Y_Size 64");
  display.setCursor(20, 50);
  display.println("VCC: 3.3V's");

  display.display();
}
//Page 4 When Encoder is Pressed
void wireCount() {
  display.clearDisplay();
  display.setCursor(10, 5);
  display.print("Wire Count:");

  display.setCursor(20, 35);
  display.println("Wires: 17");

  display.display();
}
//Page 5 When Encoder is Pressed
void components() {
  display.clearDisplay();
  display.setCursor(10, 5);
  display.print("Components:");

  display.setCursor(20, 20);
  display.println("1. 0.96 OLED");
  display.setCursor(20, 35);
  display.println("2. MPU-6050");
  display.setCursor(20, 50);
  display.println("3. Rotary Encoder");

  display.display();
}
//Page 6 When Encoder is Pressed
void aboutPage() {
  display.clearDisplay();
  display.setCursor(10, 5);
  display.print("About Page:");

  display.setCursor(20, 20);
  display.println("Menu: 6");
  display.setCursor(20, 35);
  display.println("Storage: 326600");
  display.setCursor(20, 50);
  display.println("Device: ESP32");

  display.display();
}
//Rotary Encoder Push and Page Logic
void rotaryClicked() {
  //The Rotary Encoder Push
  if (digitalRead(pinSW) == LOW) {
    // 2 Sec Loading Page
    loadingPage();

    if (menuCounter == 1){
      while (digitalRead(pinSW) == HIGH) {        
        accelometerData();
        accelometerPage();
        delay(100);
      }
      loadingPage();
    }
  
    if (menuCounter == 2){
      while (digitalRead(pinSW) == HIGH) {
        calibrationPage();
      }
      loadingPage();
    }

    if (menuCounter == 3){
      while (digitalRead(pinSW) == HIGH) {
        displayStats();
      }
      loadingPage();
    }
    if (menuCounter == 4){
      while (digitalRead(pinSW) == HIGH) {
        wireCount();
      }
      loadingPage();
    }
    if (menuCounter == 5){
      while (digitalRead(pinSW) == HIGH) {
        components();
      }
      loadingPage();
    }
    if (menuCounter == 6){
      while (digitalRead(pinSW) == HIGH) {
        aboutPage();
      }
      loadingPage();
    }
  }
}
//Rotary Encoder Turn Logic
void rotaryTurned() {
  byte currentEncoderState = (digitalRead(pinCLK) << 1) | digitalRead(pinDT);
  //Check if the Rotary Encoder dial has turned
  if (currentEncoderState != encoderStateBuffer) {
    if(encoderStateBuffer == 3 && currentEncoderState == 1) {
      menuCounter++;
    }
    else if (encoderStateBuffer == 3 && currentEncoderState == 2) {
      menuCounter--;
    }
    if(menuCounter > totalMenuItems) {
      menuCounter = 1;
    }
    if(menuCounter < 1) {
      menuCounter = totalMenuItems;
    }
  }
  encoderStateBuffer = currentEncoderState;
}


void setup() {
  Serial.begin(9600);

  //Encoder Pin Setups
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  //Start up the OLED Screen
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //Read the starting state of the clock pin
  lastCLKState = digitalRead(pinCLK);

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();

}

void loop() {
  //Rotary Dial Turn
  rotaryTurned();
  //Main Menu and Arrow
  mainMenuOne();
  mainMenuTwo();
  //The Rotary Encoder Push
  rotaryClicked();
}












