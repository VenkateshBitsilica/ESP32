#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid = "Venky";
const char* password = "11223344";

void server_page()
{
  String html = "<html><head><title>ESP32 Server</title>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "</head><body><h1>ESP32 Web Server Page Created<\h1>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", server_page);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
