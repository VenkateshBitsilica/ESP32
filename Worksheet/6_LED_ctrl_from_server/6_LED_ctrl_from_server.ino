#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "Venky";
const char* password = "11223344";

// Create WebServer on port 80
WebServer server(443);

// LED pin and status
const int ledPin = 2;
bool ledStatus = false;  // false = OFF, true = ON

// Function to build the web page
String getHTML() {
  String ledState = ledStatus ? "ON" : "OFF";
  String buttonLabel = ledStatus ? "Turn OFF" : "Turn ON";

  String html = "<html><head><title>ESP32 LED Control</title></head><body>";
  html += "<h2>ESP32 LED Control Web Page</h2>";
  html += "<p>LED Status: <strong>" + ledState + "</strong></p>";
  html += "<form action=\"/toggle\" method=\"POST\">";
  html += "<button type=\"submit\">" + buttonLabel + "</button>";
  html += "</form>";
  html += "</body></html>";

  return html;
}

// Handle the root page
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

// Handle LED toggle request
void handleToggle() {
  ledStatus = !ledStatus;  // Toggle LED status
  digitalWrite(ledPin, ledStatus ? HIGH : LOW);  // Set physical LED
  server.sendHeader("Location", "/");  // Redirect back to home page
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Start with LED OFF

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected! IP address:");
  Serial.println(WiFi.localIP());

  // Define server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_POST, handleToggle);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
   server.handleClient(); 
}
