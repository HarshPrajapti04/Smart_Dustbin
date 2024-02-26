#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>  // Include the servo library

const char* ssid = "ESP32";
const char* password = "12345678";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

const int triggerPin = 13;  // Ultrasonic sensor trigger pin
const int echoPin = 12;     // Ultrasonic sensor echo pin
long duration;
int distance;

const int triggerPin2 = 21;  // Ultrasonic sensor trigger pin
const int echoPin2 = 19;     // Ultrasonic sensor echo pin
long duration2;
int distance2;

Servo myservo;  // Create servo object to control the servo
int pos = 0;

// Variable to store the servo position

void setup() {
  Serial.begin(115200);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(4, OUTPUT);

  //ir sensor

  myservo.attach(14);  // Servo motor is attached to pin 14

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();

  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Ultrasonic sensor code to measure distance
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Ultrasonic 2 sensor code to measure distance

  digitalWrite(triggerPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  Serial.println(distance2);



  if (distance > 2.5) {
    // If the IR sensor detects an object (assuming high means detection)
    if (distance2 < 10) {
      Serial.println("high");
      myservo.write(176.2);  // Rotate the servo by 90 degrees
      delay(1000);         // Wait for the servo to move
      // Turn on LED connected to pin 4
    } else {
      myservo.write(0);  // Return the servo to 0 degrees
                         // Turn off LED connected to pin 4
    }
  }

  if (distance < 2.5) {
      digitalWrite(4, HIGH);
    // myservo.write(180);  // Rotate the servo by 90 degrees
    delay(1000);         // Wait for the servo to move
    // digitalWrite(4, HIGH);
    // Turn on LED connected to pin 4
  } else {
    digitalWrite(4, LOW);
    myservo.write(0);
    // digitalWrite(4, LOW);
    // Return the servo to 0 degrees
    // Turn off LED connected to pin 4
  }
}

void handle_OnConnect() {
  String html = SendHTML();
  server.send(200, "text/html", html);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}



String SendHTML() {


  String ptr = "<!DOCTYPE html><html>";
  ptr += "<head><meta http-equiv=\"refresh\" content=\"2\"/>";  // Refresh page every 1 seconds
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"/>";
  ptr += "<title>ESP32 Web Server</title>";
  ptr += "<style>";
  // Animated background gradient
  ptr += "body {";
  ptr += "  background: linear-gradient(-45deg, #ee7752, #e73c7e, #23a6d5, #23d5ab);";
  ptr += "  background-size: 400% 400%;";
  ptr += "  animation: gradient 15s ease infinite;";
  ptr += "  height: 100vh;";
  ptr += "}";

  ptr += "@keyframes gradient {";
  ptr += "  0% { background-position: 0% 50%; }";
  ptr += "  50% { background-position: 100% 50%; }";
  ptr += "  100% { background-position: 0% 50%; }";
  ptr += "}";
  ptr += "body { background-position: center center; background-repeat: no-repeat; background-size: cover; font-family: Arial, sans-serif; margin: 0; padding: 0; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; box-sizing: border-box; }";
  ptr += ".container { width: 100%; height: 100%; display: flex; flex-direction: column; opacity: 20; background-image: url('/data/bg.jpg');  background-position: center center; background-repeat: no-repeat; background-size: cover; box-sizing: border-box; text-transform: uppercase; padding: 10px; text-align: center; align-items: center; justify-content: center; }";
  ptr += "h1 { font-size: 50px; text-transform: capitalize; color: black; padding: 2px; }";
  ptr += "p { padding: 2px; font-weight: bold; margin-bottom: 10px; }";
  ptr += "h3 { font-size: 14px; color: black; margin-top: -20px; margin-bottom: 10px; }";
  ptr += ".box { width: 500px; height: 500px; border: 1px solid white; border-radius: 10px; padding: 0px 30px 0px 20px; box-shadow: inset; display: flex; align-items:center; justify-content:center; gap: 6px; background: rgba(193, 188, 188, 0.46); backdrop-filter: saturate(180%) blur(10px); flex-direction: column; }";
  ptr += ".items { font-size: 20px; display: flex; flex-direction: column; align-items:start; justify-content: center; margin-top: 40px; color: blue; }";
  ptr += ".on { color: red; }";
  ptr += ".off { color: green; }";

  // Media Query for Small (sm) devices (phones)
  ptr += "@media only screen and (max-width: 600px) {";
  ptr += "  .box { width: 90%; }";
  ptr += "  h1 { font-size: 30px; }";
  // Add more styles for small screens
  ptr += "}";

  // Media Query for Medium (md) devices (tablets)
  ptr += "@media only screen and (min-width: 601px) and (max-width: 768px) {";
  ptr += "  .box { width: 80%; }";
  ptr += "  h1 { font-size: 40px; }";
  // Add more styles for medium screens
  ptr += "}";

  // Media Query for Large (lg) devices (small laptops)
  ptr += "@media only screen and (min-width: 769px) and (max-width: 1024px) {";
  ptr += "  .box { width: 70%; }";
  ptr += "  h1 { font-size: 45px; }";
  // Add more styles for large screens
  ptr += "}";

  // Media Query for Extra Large (xl) devices (laptops/desktops)
  ptr += "@media only screen and (min-width: 1025px) and (max-width: 1200px) {";
  ptr += "  .box { width: 60%; }";
  ptr += "  h1 { font-size: 50px; }";
  // Add more styles for extra large screens
  ptr += "}";

  // Media Query for 2x Extra Large (2xl) devices (large desktops)
  ptr += "@media only screen and (min-width: 1201px) {";
  ptr += "  .box { width: 50%; }";
  ptr += "  h1 { font-size: 55px; }";
  // Add more styles for 2x extra large screens
  ptr += "}";
  ptr += "footer { text-align: center; padding: 10px; position: fixed; left: 0; bottom: 0; width: 100%; background-color: #333; color: white; }";
  ptr += "</style>";
  ptr += "</head>";
  ptr += "<body>";

  ptr += "<div class=\"container\">";

  // ptr += "<div class=\"navbar\">";
  // ptr += "  <button onclick=\"toggleServo()\">Toggle Servo</button>";
  // ptr += "</div>";

  ptr += "  <div class=\"box\">";
  ptr += "    <h1>SMART DUSTBIN</h1>";
  ptr += "    <h3>Using Access Point(AP) Mode</h3>";
  ptr += "    <div class=\"items\">";
  ptr += "      <p>Level of Garbage: ";
  ptr += distance;  // Assuming 'distance' is the level of garbage
  ptr += " cm</p>";
  if (distance < 5) {
    ptr += "      <p>Dustbin Status: <span class=\"on\">FULL</span></p>";
    ptr += "      <p>Servo Status: <span class=\"on\">Dustbin is closed</span></p>";
  } else {
    ptr += "      <p>Dustbin Status: <span class=\"off\">Empty</span></p>";
    ptr += "      <p>Servo Status: <span class=\"off\">Dustbin is Open</span></p>";
  }
  if (distance2 < 20) {
    ptr += "      <p>Dustbin Status: <span class=\"on\">Person Detected</span></p>";
 
  } else {
 
    ptr += "      <p>Servo Status: <span class=\"off\">Not Found</span></p>";
  }

  float occupiedSpace = 14 - distance;  // Distance from the sensor to the garbage
  float totalSpace = 14 - 2.5;          // Total usable space
  float percentageFull = (occupiedSpace / totalSpace) * 100;
  if (percentageFull < 0) {
    percentageFull = 0;  // To handle any negative value due to sensor error
  } else if (percentageFull >= 99) {
    percentageFull = 100;  // Limiting to 100%
    digitalWrite(4, HIGH);
  } else if (percentageFull < 100) {
    // Limiting to 100%
    digitalWrite(4, LOW);
  }
  ptr += "<p>Dustbin Fill Level: " + String(percentageFull, 2) + "%</p>";  // Display 2 decimal places
  ptr += "    </div>";
  ptr += "  </div>";
  ptr += "</div>";


  // ... [rest of your HTML code]

  // Add percentage display in your HTML



  ptr += "<div id='map' style='width:100%;height:400px;'></div>";


  // Footer section
  ptr += "<footer>";
  ptr += "<p>&copy; " + String(2024) + " Smart Dustbin Project. All rights reserved.</p>";  // Example footer content
  ptr += "</footer>";

  ptr += "<audio id='alertSound' src='/mp3.mp3'></audio>";  // Add this line in the HTML head or body

  ptr += "<script>";
  ptr += "function toggleServo() {";
  ptr += "  var xhttp = new XMLHttpRequest();";
  ptr += "  xhttp.open('GET', '/toggleServo', true);";
  ptr += "  xhttp.send();";
  ptr += "}";

  ptr += "function playAlertSound() {";
  ptr += "  var audio = document.getElementById('alertSound');";
  ptr += "  audio.play();";
  ptr += "}";
  ptr += "if (" + String(percentageFull) + " >= 100) {";
  // ptr += "  alert('Dustbin is full!');";
  ptr += "  playAlertSound();";
  ptr += "}";




  ptr += "</body></html>";
  return ptr;
}
