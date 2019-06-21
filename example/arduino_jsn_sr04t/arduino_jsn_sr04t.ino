/*Arduino example sketch to control a JSN-SR04T ultrasonic distance sensor with NewPing libary and Arduino. More info: https://www.makerguides.com */

// Include the library:
#include <NewPing.h>

// Define Trig and Echo pin:
#define trigPin 2
#define echoPin 3

// Define maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500 cm:
#define MAX_DISTANCE 400

// NewPing setup of pins and maximum distance.
NewPing sonar = NewPing(trigPin, echoPin, MAX_DISTANCE);

void setup() {
  // Open the Serial Monitor at 9600 baudrate to see ping results:
  Serial.begin(9600);
}

void loop() {
  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings:
  delay(50); 
  
  // Measure distance and print to the Serial Monitor:
  Serial.print("Distance = ");
  // Send ping, get distance in cm and print result (0 = outside set distance range):
  Serial.print(sonar.ping_cm()); 
  Serial.println(" cm");
  delay(3000);
}
