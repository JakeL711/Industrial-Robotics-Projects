#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <DHT.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// 1. ---- NETWORK LOG-IN REGISTERS ----
const char* WLAN_SSID = "WiFi_Name";
const char* WLAN_PASS = "WiFi_Password";

// 2. ---- CLOUD SECURITY PROFILE ----
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 8883
#define AIO_USERNAME "AdaFruit_Name"
#define AIO_KEY "AdaFruit_Password"

// Initialize the core underlying TCP/IP web connection engine
WiFiClientSecure client;

// Set up the master MQTT client machine, feeding it your security profile [Robot]
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// 3. ---- DATA PUBLISHING CANNONS ----
// Create the path blueprints pointing exactly to your three cloud dashboard feeds [Robot]
Adafruit_MQTT_Publish hum_feed  = Adafruit_MQTT_Publish(&mqtt, "AdaFruit_Name/feeds/humidity");
Adafruit_MQTT_Publish vib_feed  = Adafruit_MQTT_Publish(&mqtt, "AdaFruit_Name/feeds/vibration");

// DHT Sensor Config
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Main Telemetry Variables
float h;
float t;
float Spin_Z;

// MPU-6050 Raw Storage Registers
int16_t rawAccX, rawAccY, rawAccZ;
int16_t rawGyroX, rawGyroY, rawGyroZ;

// Calibration Offsets
float offsetX = 0.14;
float offsetY = 0.01;
float offsetZ = 0.02;
float offsetGyroX = 17.07;
float offsetGyroY = 1.28;
float offsetGyroZ = 0;

// ---- MQTT HARDWARE RECONNECT PROTOCOL ----
// Internet lines can drop out. This function forces the ESP32 to 
// automatically restore its cloud link without freezing the machine.
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) { return; } // If already securely connected, exit function

  Serial.print("Connecting to Industrial Cloud Broker... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // 0 means a flawless network handshake passed [Robot]
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Handshake failed. Retrying in 5 seconds...");
       Serial.println(ret);
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries == 0) {
         Serial.println("Critical Error: Cloud connection halted.");
         while (1); // Safe system stall
       }
  }
  Serial.println("Cloud Network Link Secure!");
}

void accelerometerData() {
  //Starts the Accelerometer's Register Box
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  //Request 14 Consectutive Bytes of Motion Data
  Wire.requestFrom(0x68, 14, true);

  //Glues the broken Binary Data Pack Back together
  rawAccX = Wire.read() <<8 | Wire.read();
  rawAccY = Wire.read() <<8 | Wire.read();
  rawAccZ = Wire.read() <<8 | Wire.read();

  int16_t rawTemp = Wire.read() <<8 | Wire.read();

  rawGyroX = Wire.read() <<8 | Wire.read();
  rawGyroY = Wire.read() <<8 | Wire.read();
  rawGyroZ = Wire.read() <<8 | Wire.read();

  //Converts Data to Measurements
  float G_ForceX = (rawAccX/16384.0) + offsetX;
  float G_ForceY = (rawAccY/16384.0) + offsetY;
  float G_ForceZ = (rawAccZ/16384.0) + offsetZ;

  float Spin_X = (rawGyroX/131.0) + offsetGyroX;
  float Spin_Y = (rawGyroY/131.0) - offsetGyroY;
  Spin_Z = (rawGyroZ/131.0) + offsetGyroZ;

  //Serial Prints
  Serial.print("Tilt_X: "); Serial.print(G_ForceX);
  Serial.print(" | Spin_Z: "); Serial.println(Spin_Z);
}

void setup() {
  Serial.begin(115200); 
  delay(10);
  dht.begin();

  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();

  // ----  THE CRITICAL SSL SECURITY BYPASS ----
  // This tells the ESP32 to connect to Adafruit over the secure port (8883)
  // but skips the complicated certificate file checking that causes the -1 crash!
  client.setInsecure(); 

  // Connect to local Wi-Fi router
  Serial.println(); Serial.print("Connecting to router SSID: ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nRouter connection established!");
  Serial.print("Local IP Target: "); Serial.println(WiFi.localIP());
}

void loop() {
  MQTT_connect();
  
  h = dht.readHumidity();
  accelerometerData();
  
  //Local Console Status Update
  Serial.println("\n--- STREAMING PACKET TO CLOUD ---");
  Serial.print("Sending Temp:"); Serial.println(t);

  // We call .publish() on our feed paths to fire the data straight up to the internet!
  if (! hum_feed.publish(h))  { Serial.println("Humidity upload failed."); }
  if (! vib_feed.publish(Spin_Z)) { Serial.println("Vibration upload failed."); }

  delay(5000);
}
