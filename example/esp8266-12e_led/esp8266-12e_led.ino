//#include <ESP8266WiFi.h>
//#include <RGBLED.h>
//
//#include <ezTime.h>
//#include <ArduinoJson.h>
//#include <ESP8266HTTPClient.h>
////#define COMMON_ANODE
//#define LOCALTZ_POSIX  "CET-1CEST,M3.4.0/2,M10.4.0/3"    // Time in Berlin
//
//Timezone local;
//Timezone pacific;
//
//const char* ssid = "Dicosta"; // Change to your network SSID
//const char* password = "ramo011911"; // You can change the Password depending upon your netwrk
//IPAddress ip(192, 168, 43, 9); // where xx is the desired IP Address
//IPAddress gateway(192, 168, 43, 254); // set gateway to match your wifi network
//IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your wifi network   16,14,12,13
//
//int switch1 = 16;
//int switch2 = 14;
//int switch3 = 12;
//int switch4 = 13;
//int value1, value2, value3, value4;
//
//WiFiServer server(80);
//
//void connectToWifi() {
//  delay(1000);
//  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
//  delay(1000);
//  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
//
//  WiFi.config(ip, gateway, subnet);
//  WiFi.begin(ssid, password);     //Connect to your WiFi router
//  Serial.println("");
//
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  // Wait for connection
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//  Serial.print("Netmask: ");
//  Serial.println(WiFi.subnetMask());
//  Serial.print("Gateway: ");
//  Serial.println(WiFi.gatewayIP());
//  Serial.print("MAC: ");
//  Serial.println(WiFi.macAddress());
//
//  server.begin();
//  Serial.println("Server started");
//  Serial.print("Use this URL to connect: ");
//  Serial.print("http://");
//  Serial.print(WiFi.localIP());
//  Serial.println("/");
//  value1 = value2 = value3 = value4 = LOW;
//}
//
//void setup() {
//  // Setup serial port
//  Serial.begin(115200);
//  delay(10000);
//  while (!Serial) {    // wait for Serial port to connect. Needed for native USB port only
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println();
//
//  // Connect to wifi
//  connectToWifi();
//}
//
//void loop() {
//  WiFiClient client = server.available();
//  if (!client) {
//    return;
//  }
//  Serial.println("new client");
//  while (!client.available()) {
//    delay(1);
//  }
//  String request = client.readStringUntil('\r');
//  Serial.println(request);
//  client.flush();
//  if (request.indexOf("/LED=ON1") != -1) {
//    digitalWrite(switch1, HIGH);
//    value1 = HIGH;
//  }
//  if (request.indexOf("/LED=OFF1") != -1) {
//    digitalWrite(switch1, LOW);
//    value1 = LOW;
//  }
//
//  if (request.indexOf("/LED=ON2") != -1) {
//    digitalWrite(switch2, HIGH);
//    value2 = HIGH;
//  }
//  if (request.indexOf("/LED=OFF2") != -1) {
//    digitalWrite(switch2, LOW);
//    value2 = LOW;
//  }
//  if (request.indexOf("/LED=ON3") != -1) {
//    digitalWrite(switch3, HIGH);
//    value3 = HIGH;
//  }
//  if (request.indexOf("/LED=OFF3") != -1) {
//    digitalWrite(switch3, LOW);
//    value3 = LOW;
//  }
//  if (request.indexOf("/LED=ON4") != -1) {
//    digitalWrite(switch4, HIGH);
//    value4 = HIGH;
//  }
//
//  if (request.indexOf("/LED=OFF4") != -1) {
//    digitalWrite(switch4, LOW);
//    value4 = LOW;
//  }
//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-Type: text/html");
//  client.println(""); //  do not forget this one
//  client.println("<!DOCTYPE HTML>");
//  client.println("<html>");
//  client.println("<center>");
//  client.println("<body>");
//  client.println("<body bgcolor=celeste>");
//  //client.println("<border>");
//  client.println("<border 1px solid black>");
//  //client.println("<body = bgcolor=teal>");
//  client.println("<div><font size=80><font color=’blue ’><h1>Instructables</font></h1></font></div>");
//  client.println("<div><font size=95><font color=’chocolate ’><h1><class=dotted>...Home Automation...</font></font color></h1></div>");
//  client.println("</border>");
//  client.println("<br><br>");
//  client.println("<br><br>");
//  client.println("<br>");
//  client.println("<table>");
//  client.println(" <border><table><tr><td><font size=90><font color=’chocolate ’><b>Switch 1--- </b><a href=\"LED=ON1\"><button><font size=75><font color= GREEN>ON</button></a>&nbsp;<a href=\"/LED=OFF1\"><button><font size=75><font color= RED>OFF</button></font></a><br></tr></td></table></border>");
//  client.println(" <table><tr><td><font size=95><font color=’chocolate ’><b>Switch 2--- </b><a href=\"LED=ON2\"><button><font size=75><font color= GREEN>ON</button></a>&nbsp;<a href=\"/LED=OFF2\"><button><font size=75><font color= RED>OFF</button></font></a><br></tr></td></table>");
//  client.println(" <table><tr><td><font size=95><font color=’chocolate ’><b>Switch 3--- </b><a href=\"LED=ON3\"><button><font size=75><font color= GREEN>ON</button></a>&nbsp;<a href=\"/LED=OFF3\"><button><font size=75><font color= RED>OFF</button></font></a><br></tr></td></table>");
//  client.println(" <table><tr><td><font size=95><font color=’chocolate ’><b>Switch 4--- </b><a href=\"LED=ON4\"><button><font size=75><font color= GREEN>ON</button></a>&nbsp;<a href=\"/LED=OFF4\"><button><font size=75><font color= RED>OFF</button></font></a></tr></td></table>");
//  client.println("<br><br>");
//  if (value1 == HIGH) {
//    client.print("<td><font size=60><font color=’chocolate ’>Device1 is: </font>");
//    client.print("<font size=75><font color= GREEN>ON</font>");
//
//  }
//  else {
//    client.print("<font size=75><font color=’chocolate ’>Device1 is: </font>");
//    client.print("<font size=75><font color= RED>OFF</font>");
//
//  }
//  client.println("<br>");
//  if (value2 == HIGH) {
//    client.print("<font size=75><font color=’chocolate ’>Device2 is: </font>");
//    client.print("<font size=75><font color= GREEN>ON</font>");
//
//  } else {
//    client.print("<font size=75><font color=’chocolate ’>Device2 is: </font>");
//    client.print("<font size=75><font color= RED>OFF</font>");
//
//  }
//  client.println("<br>");
//  if (value3 == HIGH) {
//    client.print("<font size=75><font color=’chocolate ’>Device3 is: </font>");
//    client.print("<font size=75><font color= GREEN>ON</font>");
//
//  } else {
//    client.print("<font size=75><font color=’chocolate ’>Device3 is: </font>");
//    client.print("<font size=75><font color= RED>OFF</font>");
//
//  }
//  client.println("<br>");
//  if (value4 == HIGH) {
//    client.print("<font size=75><font color=’chocolate ’>Device4 is: </font>");
//    client.print("<font size=75><font color= GREEN>ON</font>");
//
//  } else {
//    client.print("<font size=75><font color=’chocolate ’>Device4 is: </font>");
//    client.print("<font size=75><font color= RED>OFF</font></td></tr></table>");
//
//  }
//  client.println("</table>");
//  client.println("</body>");
//  client.println("</center>");
//  client.println("</html>");
//  delay(1);
//  Serial.println("Client disonnected");
//  Serial.println("");
//}


// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "Dicosta"; // Input your wifi network name
const char* password = "ramo011911"; // Input your wifi password

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String relay1State = "off";
String relay2State = "off";
String relay3State = "off";
String relay4State = "off";
String relay5State = "off";

// Assign output variables to GPIO pins
const int relay1 = 16; // GPIO16
const int relay2 = 14; // GPIO14
const int relay3 = 12; // GPIO12
const int relay4 = 13; // GPIO13
const int relay5 = 4;  // GPIO4

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  // Set outputs to HIGH. relay active LOW
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /16/on") >= 0)
            {
              Serial.println("GPIO 16 on");
              relay1State = "on";
              digitalWrite(relay1, LOW);
            }
            else if (header.indexOf("GET /16/off") >= 0)
            {
              Serial.println("GPIO 16 off");
              relay1State = "off";
              digitalWrite(relay1, HIGH);
            }
            else if (header.indexOf("GET /14/on") >= 0) {
              Serial.println("GPIO 14 on");
              relay2State = "on";
              digitalWrite(relay2, LOW);
            }
            else if (header.indexOf("GET /14/off") >= 0) {
              Serial.println("GPIO 14 off");
              relay2State = "off";
              digitalWrite(relay2, HIGH);
            }
            else if (header.indexOf("GET /12/on") >= 0)
            {
              Serial.println("GPIO 12 on");
              relay3State = "on";
              digitalWrite(relay3, LOW);
            }
            else if (header.indexOf("GET /12/off") >= 0)
            {
              Serial.println("GPIO 12 off");
              relay3State = "off";
              digitalWrite(relay3, HIGH);
            }
            else if (header.indexOf("GET /13/on") >= 0) {
              Serial.println("GPIO 13 on");
              relay4State = "on";
              digitalWrite(relay4, LOW);
            }
            else if (header.indexOf("GET /13/off") >= 0) {
              Serial.println("GPIO 13 off");
              relay4State = "off";
              digitalWrite(relay4, HIGH);
            }
            else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              relay5State = "on";
              digitalWrite(relay5, LOW);
            }
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              relay5State = "off";
              digitalWrite(relay5, HIGH);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 12px 24px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>NodeMCU Web Server</h1>");

            // Display current state, and ON/OFF buttons for GPIO 16
            client.println("<p>Relay 16 - State " + relay1State + "</p>");
            // If the relay1State is off, it displays the ON button
            if (relay1State == "off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 14
            client.println("<p>Relay 14 - State " + relay2State + "</p>");
            // If the relay2State is off, it displays the ON button
            if (relay2State == "off") {
              client.println("<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 12
            client.println("<p>Relay 12 - State " + relay3State + "</p>");
            // If the relay3State is off, it displays the ON button
            if (relay3State == "off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 13
            client.println("<p>Relay 13 - State " + relay4State + "</p>");
            // If the relay4State is off, it displays the ON button
            if (relay4State == "off") {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 4
            client.println("<p>Relay 4 - State " + relay5State + "</p>");
            // If the relay5State is off, it displays the ON button
            if (relay5State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
