// Load Wi-Fi library
#include <WiFi.h>
// Load Servo library
#include <ESP32Servo.h>

// create servo object to control a servo
Servo myservo;

// Replace with your network credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID;
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;


// Auxiliar variables to store the current output state
String outputServoPin = "off";

// GPIO the servo is attached to
static const int servoPin = 13;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
Serial.begin(115200);

// attaches the servo on the servoPin to the servo object
myservo.attach(servoPin);  

 // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
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
if (header.indexOf("GET /on") >= 0) {
Serial.println("GPIO 13 on");
outputServoPin = "on";
myservo.write(90);
Serial.println(90); 


} else if (header.indexOf("GET /off") >= 0) {
Serial.println("GPIO 13 off");
outputServoPin = "off";
myservo.write(0);
Serial.println(0); 
}


// Display the HTML web page
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
// CSS to style the on/off buttons 
// Feel free to change the background-color and font-size attributes to fit your preferences
client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".button2 {background-color: #555555;}</style></head>");

// Web Page Heading
client.println("<body><h1>Spray Servo Server</h1>");

// Display current state, and ON/OFF buttons for GPIO 13 
client.println("<p>GPIO 13 - State " + outputServoPin + "</p>");
// If the outputServoPin is off, it displays the ON button       
if (outputServoPin=="off") {
client.println("<p><a href=\"/on\"><button class=\"button\">ON</button></a></p>");
} else {
client.println("<p><a href=\"/off\"><button class=\"button button2\">OFF</button></a></p>");
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