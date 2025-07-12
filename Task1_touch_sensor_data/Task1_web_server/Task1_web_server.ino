#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Venky";
const char* password = "11223344";

WebServer server(80);
String sensorStatus = "Waiting...";

void handleRoot() {
  String html = "<html><head><title>ESP32 Touch Sensor</title>";
  html += "<meta http-equiv='refresh' content='2'>"; 
  html += "</head><body>";
  html += "<h2>ESP32 Touch Sensor Status:</h2>";
  html += "<h1 style='color:red;'>" + sensorStatus + "</h1>";
  html += "</body></html>";

  /*server.send(statusCode, contentType, content);
  server.send(statusCode, contentType, content, content_length);
  */
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());

/*server.on(path, handler_function);
server.on(path, HTTPMethod, handler_function);
server.on(path, HTTPMethod, handler_function, pre_condition_function);
*/
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  uint8_t t = touchRead(T0);  // GPIO4
  Serial.println(t);

  if (t < 30) {
    sensorStatus = "Sensor touched!";
  } else {
    sensorStatus = "Not touched";
  }

  delay(500);
}
