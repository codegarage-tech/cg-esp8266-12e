#include <NewPing.h>
#include <RGBLED.h>
#include <ezTime.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
//#define COMMON_ANODE
// Time in Berlin
#define LOCALTZ_POSIX  "CET-1CEST,M3.4.0/2,M10.4.0/3"

Timezone local;
Timezone pacific;

const char* ssid     = "Dicosta";
const char* password = "ramo011911";
//const char* host = "iot-cuddle.000webhostapp.com";
//int gpio = 1;
//int GPIO0 = 0, GPIO2 = 2;

int GPIO4 = 4, GPIO12 = 12, GPIO13 = 13, GPIO14 = 14, GPIO16 = 16;
// Define maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500 cm:
int MAX_DISTANCE = 400;
// NewPing setup of pins and maximum distance.
NewPing sonar = NewPing(GPIO13, GPIO12, MAX_DISTANCE);

//TX = 1, RX = 3;

// Declare an RGBLED instanced named rgbLed
// Red, Green and Blue LED legs are connected to PWM pins 11,9 & 6 respectively
// In this example, we have a COMMON_ANODE LED, use COMMON_CATHODE otherwise
//RGBLED rgbLed(TX, GPIO2, RX, COMMON_ANODE);

void connectToWifi() {
  delay(1000);
  // Prevents reconnection issue (taking too long to connect)
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);
  // Connect to your WiFi router
  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
}

void setup() {
  // Setup serial port
  Serial.begin(115200);
  delay(10000);
  // wait for Serial port to connect. Needed for native USB port only
  while (!Serial) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Dealing with current date-time
  local.setPosix(LOCALTZ_POSIX);
  local.setTime(compileTime());
  Serial.print(F("Local time   :  "));
  Serial.println(local.dateTime());
  pacific.setPosix(F("PST+8PDT,M3.2.0/2,M11.1.0/2"));
  Serial.print(F("Pacific time :  "));
  Serial.println(pacific.dateTime());
  Serial.print(F("UTC          :  "));
  Serial.println(UTC.dateTime());

  // Setup different pins
  pinMode(GPIO4, OUTPUT);
  pinMode(GPIO14, OUTPUT);
  pinMode(GPIO16, OUTPUT);
  Serial.println("Set different pin as output");


  //  pinMode(GPIO0, OUTPUT);
  //  //////////////////////
  ////  pinMode(TX, OUTPUT);
  //  pinMode(GPIO2, OUTPUT);
  ////  pinMode(RX, OUTPUT);
  //  ///////////////////////
  //  digitalWrite(GPIO0, HIGH);
  //  digitalWrite(GPIO2, HIGH);
  //  ///////////////////
  //  //  analogWrite(TX, 1023);
  //  //  analogWrite(GPIO2, 1023);
  //  //  analogWrite(RX, 1023);
  /////////////////////////
  Serial.println();

  // Connect to wifi
  connectToWifi();
}

void loop() {
  String wifi_status = "", distance = "";
  // Current wifi status
  wifi_status = (WiFi.status() == WL_CONNECTED) ? "on" : "off";

  // Do server request task
  if (WiFi.status() == WL_CONNECTED) {
    // Light up wifi signal
    digitalWrite(GPIO4, HIGH);
    delay(10);
    Serial.println("GPIO 4 is On.");

    // Current distance
    distance = sonar.ping_cm();

    HTTPClient http;
    String baseUrl = "http://iot-cuddle.000webhostapp.com/iot/api/led/SmartMotor.php";
    //http://iot-cuddle.000webhostapp.com/iot/api/led/SmartMotor.php?wifi_status=on&distance=15
//    String url = "http://iot-cuddle.000webhostapp.com/iot/api/led/SmartMotor.php?" + "wifi_status=" + wifi_status + "&distance=" + distance;
    baseUrl += "?wifi_status=";
    baseUrl += wifi_status;
    baseUrl += "&distance=";
    baseUrl += distance;

    Serial.print("Requesting URL: ");
    Serial.println(baseUrl);

    Serial.print("[HTTP] begin...\n");
    http.begin( baseUrl);

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        Serial.println("[HTTP] GET... success");
        String payload = http.getString();

        if (payload != NULL && payload.length() > 0) {
          Serial.println("response: " + payload);

          // Parse JSON
          StaticJsonBuffer<1000> jsonBuffer;
          JsonObject& json_parsed = jsonBuffer.parseObject(payload);
          if (!json_parsed.success()) {
            Serial.println("parseObject() failed");
            return;
          }

          String gpioId14 = json_parsed["led"][2]["id"];
          String gpioStatus14 = json_parsed["led"][2]["status"];
          Serial.println("gpioId14: " + gpioId14);
          Serial.println("gpioStatus14: " + gpioStatus14);

          String gpioId16 = json_parsed["led"][3]["id"];
          String gpioStatus16 = json_parsed["led"][3]["status"];
          Serial.println("gpioId16: " + gpioId16);
          Serial.println("gpioStatus16: " + gpioStatus16);

          // Process json data
          // Controll relay 2
          if (gpioStatus14 == "on") {
            digitalWrite(GPIO14, LOW);
            delay(100);
            Serial.println("GPIO 14 is On..!");
          } else if (gpioStatus14 == "off") {
            digitalWrite(GPIO14, HIGH);
            delay(100);
            Serial.println("GPIO 14 is Off..!");
          }

          // Control relay 1
          if (gpioStatus16 == "on") {
            digitalWrite(GPIO16, LOW);
            delay(100);
            Serial.println("GPIO 16 is On..!");
          } else if (gpioStatus16 == "off") {
            digitalWrite(GPIO16, HIGH);
            delay(100);
            Serial.println("GPIO 16 is Off..!");
          }
        } else {
          Serial.printf("Got null or empty response from server");
        }
      } else {
        Serial.printf("Server request code is positive, error: %s\n", http.errorToString(httpCode).c_str());
      }
    } else {
      Serial.printf("Server request code is negative, error: %s\n", http.errorToString(httpCode).c_str());
    }

    // Close server connection
    http.end();
  } else {
    // Light off wifi signal
    Serial.println("There is no internet connection");
    digitalWrite(GPIO4, HIGH);
    delay(10);
    Serial.println("GPIO 4 is off.");
  }
  /*
    //////////////////////////////////////////
    //  //Control wifi led signal
    //   long rssi = WiFi.RSSI();
    //   Serial.println("rssi: " + rssi);
    //   int bars = getBarsSignal(rssi);
    //   Serial.println("bars: " + bars);
    //   setWifiSignlaLed(bars);

    //static int previousQuality = -1;
    //  int quality = getQuality();
    //  if (quality != previousQuality) {  // If the quality changed since last print, print new quality and RSSI
    //    if (quality != -1){
    //      Serial.printf("WiFi Quality:\t%d\%\tRSSI:\t%d dBm\r\n", quality, WiFi.RSSI());
    int bars = getBarsSignal(WiFi.RSSI());
    //      Serial.printf("bars: \t%d\r\n" , bars);
    if ((bars == 4) || (bars == 5)) {
    Serial.println("Wifi Signal is good: 4 or 5");
    //Set the RGBLED to show GREEN only
    //    rgbLed.writeRGB(0, 255, 0);
    analogWrite(GPIO2, 255);
    analogWrite(RX,1023);
    analogWrite(TX,1023);
    } else if (bars == 3) {
    Serial.println("Wifi Signal is medium: 3");
    //Set the RGBLED to show BLUE only
    //    rgbLed.writeRGB(0, 0, 255);
    analogWrite(GPIO2, 1023);
    analogWrite(RX,1023);
    analogWrite(TX,255);
    } else if ((bars == 1) || (bars == 2)) {
    Serial.println("Wifi Signal is low: 1 or 2");
    //Set the RGBLED to show YELLOW (RED & GREEN) only
    //    rgbLed.writeRGB(255, 255, 0);
    analogWrite(GPIO2, 1023);
    analogWrite(RX,255);
    analogWrite(TX,1023);
    } else if (bars == 0) {
    Serial.println("Wifi Signal is not available: 0");
    //Set the RGBLED to show RED only
    //    rgbLed.writeRGB(255, 0, 0);
    analogWrite(GPIO2, 1023);
    analogWrite(RX,1023);
    analogWrite(TX,1023);
    }
    //    }
    //    previousQuality = quality;
    //  }
  */

  //////////////////////////////////////////
  Serial.println("Wait 3s before next round...");
  delay(3000);
}

//void setColor(int red, int green, int blue)
//{
//  #ifdef COMMON_ANODE
//    red = 255 - red;
//    green = 255 - green;
//    blue = 255 - blue;
//  #endif
//  analogWrite(redPin, red);
//  analogWrite(greenPin, green);
//  analogWrite(bluePin, blue);
//}

int getBarsSignal(long rssi) {
  // 5. High quality: 90% ~= -55db
  // 4. Good quality: 75% ~= -65db
  // 3. Medium quality: 50% ~= -75db
  // 2. Low quality: 30% ~= -85db
  // 1. Unusable quality: 8% ~= -96db
  // 0. No signal

  int bars = 0;
  if (WiFi.status() == WL_CONNECTED) {
    if (rssi > -55) {
      bars = 5;
    } else if (rssi < -55 & rssi > -65) {
      bars = 4;
    } else if (rssi < -65 & rssi > -75) {
      bars = 3;
    } else if (rssi < -75 & rssi > -85) {
      bars = 2;
    } else if (rssi < -85 & rssi > -96) {
      bars = 1;
    } else {
      bars = 0;
    }
  }

  return bars;
}

///*
//   Return the quality (Received Signal Strength Indicator)
//   of the WiFi network.
//   Returns a number between 0 and 100 if WiFi is connected.
//   Returns -1 if WiFi is disconnected.
//*/
//int getQuality() {
//  if (WiFi.status() != WL_CONNECTED)
//    return -1;
//  int dBm = WiFi.RSSI();
//  if (dBm <= -100)
//    return 0;
//  if (dBm >= -50)
//    return 100;
//  return 2 * (dBm + 100);
//}

//void fade(int pin) {
//  for (int u = 0; u < 1024; u++) {
//    analogWrite(pin, 1023 - u);
//    delay(1);
//  }
//  for (int u = 0; u < 1024; u++) {
//    analogWrite(pin, u);
//    delay(1);
//  }
//}
