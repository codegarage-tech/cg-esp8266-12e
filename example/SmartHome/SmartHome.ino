#include <DHT.h>
#include <Adafruit_Sensor.h>

#include <SoftwareSerial.h>
#include <ezTime.h>
#include <ESP8266WiFi.h>
#define LOCALTZ_POSIX  "CET-1CEST,M3.4.0/2,M10.4.0/3"
#define DHTTYPE DHT11

Timezone local;
Timezone pacific;

const char* ssid     = "Dicosta";
const char* password = "ramo011911";

int WIFI_LED_PIN = 4, DHT_PIN = 12, LDR_PIN = A0, BLUETOOTH_LED_PIN = 14, DHT_LED_PIN = 16, LDR_LED_PIN = 13;
float temperature = 0, humidity = 0, feelsLike = 0;
char serialData;

DHT dht(DHT_PIN, DHTTYPE);

// Bluetooth varialbes
//SoftwareSerial bluetooth(3, 1); // RX, TX

void connectToWifi() {
  delay(1000);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
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
  Serial.begin(9600);
  delay(5000);
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
  pinMode(LDR_PIN, INPUT);
  pinMode(DHT_PIN, INPUT);
  pinMode(WIFI_LED_PIN, OUTPUT);
  pinMode(DHT_LED_PIN, OUTPUT);
  pinMode(LDR_LED_PIN, OUTPUT);
  pinMode(BLUETOOTH_LED_PIN, OUTPUT);
  Serial.println("Set different pin as output");

  // Set switch off before connecting to wifi
  digitalWrite(WIFI_LED_PIN, LOW);
  digitalWrite(DHT_LED_PIN, LOW);
  digitalWrite(LDR_LED_PIN, LOW);
  digitalWrite(BLUETOOTH_LED_PIN, LOW);

  // Start temperature sensor
  dht.begin();

  // Connect to wifi
  connectToWifi();
}

void loop() {
  /////////////// Bluetooth manipulation ////////////////
  if (Serial.available() > 0) {
    serialData = Serial.read();
    Serial.print(F("Serial data is : "));
    Serial.print(serialData);
    Serial.println(F(""));
    if (serialData == '1') {
      digitalWrite(BLUETOOTH_LED_PIN, HIGH);
      Serial.println(F("Bluetooth led is on"));

    } else if (serialData == '0') {
      digitalWrite(BLUETOOTH_LED_PIN, LOW);
      Serial.println(F("Bluetooth led is off"));
    }
  }

  ////////////  DHT manipulation ///////////////////
  temperature = dht.readTemperature();
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C"));
  Serial.println(F(""));
  humidity = dht.readHumidity();
  Serial.print(F("humidity: "));
  Serial.print(humidity);
  Serial.print(F("%"));
  Serial.println(F(""));
  //    if (isnan(temperature) || isnan(humidity)) {
  //      Serial.println(F("Failed to read from DHT sensor!"));
  //      return;
  //    }
  feelsLike = dht.computeHeatIndex(temperature, humidity, false);
  Serial.print(F("feelsLike: "));
  Serial.print(feelsLike);
  Serial.print(F("°C"));
  Serial.println(F(""));
  if (temperature > 32) {
    digitalWrite(DHT_LED_PIN, HIGH);
    Serial.println("It's hot Outside, Temperature led is on");
  } else {
    digitalWrite(DHT_LED_PIN, LOW);
    Serial.println("It's cool Outside, Temperature led is off");
  }

  ///////////// LDR manipulation ///////////////
  int LDRValue = analogRead(LDR_PIN);
  Serial.print(F("LDRValue: "));
  Serial.print(LDRValue);
  Serial.println(F(""));
  //  if (LDRValue <= 700){
  if (LDRValue <= 200) {
    digitalWrite(LDR_LED_PIN, HIGH);
    Serial.println("It's Dark Outside, LDR led is on");
  } else {
    digitalWrite(LDR_LED_PIN, LOW);
    Serial.println("It's Bright Outside, LDR led is off");
  }

  /////////////// Wifi led manipulation ////////////////
  int bars = getBarsSignal(WiFi.RSSI());
  Serial.printf("Wifi bars is: %d" , bars);
  Serial.println("");
  if (bars <= 1) {
    WiFi.disconnect();
    digitalWrite(WIFI_LED_PIN, LOW);
    connectToWifi();
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wifi is connected");
    digitalWrite(WIFI_LED_PIN, HIGH);
    Serial.println("WIFI led is On.");
  } else {
    Serial.println("There is no internet connection");
    Serial.println("Wifi is connected");
    digitalWrite(WIFI_LED_PIN, LOW);
    Serial.println("WIFI led is off.");
  }

  delay(3000);
  Serial.println("..........................................................");
}

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
