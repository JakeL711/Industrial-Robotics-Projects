#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "WiFi_Username";
const char* password = "WiFi_Password";

WiFiServer server(80);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//System Variables
String header;
String machineMessage = "SYSTEM READY";
int currentMotorSpeed = 0;
int currentPitchValue = 0;
//Pins
int pinENA = 27;
int pinLED = 26;
int INOne = 12;
int INTwo = 14;
int pinBuzzer = 32;


//Setups up the basic website style and the slider/text box:
void webPageSetup(WiFiClient &client) {
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<style>html { font-family: sans-serif; text-align: center; background-color: #222; color: #fff;}");
  client.println(".slider { width: 80%; max-width: 400px; height: 25px; background: #d3d3d3; outline: none; opacity: 0.7;}");
  client.println(".text-box { padding: 10px; font-size: 18px; width: 60%; max-width: 300px;}</style></head>");
  
  client.println("<body><h1>ADVANCED PROCESS CONTROL</h1>");

}
//Shows the subtexts that include some of the values the variables are at:
void subTexts(WiFiClient &client) {
  client.println("<hr>");
  client.println("<h3>LIVE SYSTEM TELEMETRY");
  client.println("<p>Current Target Speed: <strong>" + String(currentMotorSpeed) + " PWM Units</strong></p>");
  client.println("<p>Active Panel Screen Log: <strong>\"" + machineMessage + "\"<strong></p>");
  client.println("<hr>");
}
//Makes the slider for the motors speed
void motorSlider(WiFiClient &client) {
  client.println("<h3>CONVEYOR THROTTLE PANEL");
  client.println("<p><input type=\"range\" min=\"127\" max=\"255\" value=\"" + String(currentMotorSpeed) + "\" class=\"slider\" onchange=\"location.href='/setSpeed?value='+this.value\"></p>");
}
//Makes the text box for LED Brightness
void LEDTextBox(WiFiClient &client) {
  client.println("<h3>BROADCAST TEXT TO PANEL SCREEN (0-255)</h3>");
  client.println("<form action=\"/submitText\" method=\"get\">");
  client.println("<input type=\"text\" name=\"msg\" placeholder=\"Type alert here...\" class=\"text-box\"><br><br>");
  client.println("<input type=\"submit\" value=\"SEND TO MACHINE\" style=\"padding: 10px 20px; font-size: 16px;\">");
  client.println("</form>");
}
//Make the sliders for the Sound pitch
void buzzerSlider(WiFiClient &client) {
  client.println("<h3>BUZZER PANEL");
  client.println("<p><input type=\"range\" min=\"0\" max=\"2000\" value=\"" + String(currentPitchValue) + "\" class=\"slider\" onchange=\"location.href='/setPitch?value='+this.value\"></p>");
  client.println("</body></html>");
  client.println();
}
//Checks the Value From the Website's Slider, and analogWrites the pinLED
void pinLEDValue(WiFiClient &client) {
  int index = header.indexOf("GET /submitText?msg=");
  
  if (index >= 0) {
    String rawMsg = header.substring(index + 20, header.indexOf(" HTTP"));
    rawMsg.replace("+", " ");
    machineMessage = rawMsg;

    //Turns the LED on
    int ledValue = machineMessage.toInt();
    if (ledValue >= 255) {
      ledValue = 255;
    }
    if (ledValue <= 0) {
      ledValue = 0;
    }
    analogWrite(pinLED, ledValue);

    //Print the wireless typed text to lcd display
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("LED BRIGHTNESS");
    lcd.setCursor(0,1); lcd.print(machineMessage);
  }
}
//Check the Value From the Websites Textbox, and analogWrites the pinENA
void pinENAValue(WiFiClient &client) {
  int index = header.indexOf("GET /setSpeed?value=");
  
  if (index >= 0) {
    // Extract the number text string right after the equals sign
    String speedString = header.substring(index + 20, header.indexOf(" HTTP"));
    currentMotorSpeed = speedString.toInt(); // Convert text to math integer
              
    // If speed is 127, safely kill the direction lines, 
    if(currentMotorSpeed <= 127) {
      digitalWrite(INOne, LOW);
      digitalWrite(pinENA, LOW); // Turn off
    } else {
      digitalWrite(INOne, HIGH);
      analogWrite(pinENA, currentMotorSpeed);  // Turn on 
    }
  }
}
//Check the Value Fron the Websites Textbox, and analogWrites the pinBuzzer
void pinBuzzerValue(WiFiClient &client) {
  int index = header.indexOf("GET /setPitch?value=");
  
  if (index >= 0) {
    String pitchString = header.substring(index + 20, header.indexOf(" HTTP"));
    currentPitchValue = pitchString.toInt();

    if (currentPitchValue == 0) {
      noTone(pinBuzzer);
    } else {
      tone(pinBuzzer, currentPitchValue);
    }
  }
}


void setup() {
  delay(3000);
  Serial.begin(115200);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);

  lcd.init();
  lcd.backlight();

  pinMode(pinENA, OUTPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(INOne, OUTPUT);
  pinMode(INTwo, OUTPUT);

  digitalWrite(INOne, HIGH);
  digitalWrite(INTwo, LOW);

  lcd.setCursor(0,0);
  lcd.print("BOOTING NETWORK");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.setCursor(0,0); lcd.print("WiFi Connected!");
  lcd.setCursor(0,1); lcd.print(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client Logged In");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            //Send Universal Web Protocol Header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Part 1: HTML Form Values:
            //Action A: Check if the client moved the HTML Slider
            pinENAValue(client);
            
            //Action B: Check if the Client typed into the HTML Input Box
            pinLEDValue(client);
            
            //Action C: 
            pinBuzzerValue(client);

            //Part 2: Build the Website:
            //Webpage's style setup
            webPageSetup(client);
            
            //Displays Current Live System Variables
            subTexts(client);

            //Motor Speed Slider Bar
            motorSlider(client);

            //LED Text Box
            LEDTextBox(client);

            //Speaker Sound Slider
            buzzerSlider(client);

            //Clear data before break
            header = "";

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}

