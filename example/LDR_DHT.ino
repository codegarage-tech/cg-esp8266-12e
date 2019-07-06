// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
int LED1 = 14;
int LED2 = 15;
//int LED3 = 10;

#define DHTPIN 12    // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
char data_received;
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  dht.begin();
}

void loop() {
  if (Serial.available() > 0) /* If data is available on serial port */
  {

    data_received = Serial.read();
  }


  while (data_received == '1')
  {
    // Wait a few seconds between measurements.
    delay(2000);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float H = h;
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
      //if (Serial.available())
      //data_received = Serial.read();
      //if (data_received!='1')
      //goto endloop;

    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));

    if (h > 35)
    {
      H = dht.readHumidity();

      analogWrite(LED1, 400 + (h - H) * 100);

    }
    else {
      H = dht.readHumidity();

      analogWrite(LED1, 400 - (H - h) * 100);
    }

    if (Serial.available() > 0) /* If data is available on serial port */
    {

      data_received = Serial.read();
      Serial.print("done");
    }
    if (data_received!='1')
    goto nothing;

  }





if (data_received == '2')
  {
    int sensorValue = analogRead(A0);


    analogWrite(LED2, sensorValue);


    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    //float voltage = sensorValue * (5.0 / 1023.0);

    // print out the value you read:
    Serial.println(sensorValue);

  }

nothing :
{
}
}
