#include <WiFi.h>
#include <WebServer.h>
#include <time.h>

const char* ssid = "Venky";
const char* password = "11223344";

WebServer server(80);

#define MAX_HISTORY 10
String history[MAX_HISTORY];
int historyIndex = 0;

void addToHistory(String msg) {
  if (historyIndex >= MAX_HISTORY) historyIndex = 0;
  history[historyIndex++] = msg;
}

String getCurrentTime() {
  struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
    return "Time not set";
  }
  char timeStr[30];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeStr);
}

void handleRoot() {
  String html = "<html><head><title>ESP32 Touch Sensor</title>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "</head><body>";
  html += "<h2>Touch History:</h2><ul>";

  for (int i = 0; i < MAX_HISTORY; i++) {
    int index = (historyIndex + i) % MAX_HISTORY;
    if (history[index] != "") {
      html += "<li>" + history[index] + "</li>";
    }
  }

  html += "</ul></body></html>";
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

  configTime(19800, 0, "pool.ntp.org", "time.nist.gov"); 

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  uint8_t t = touchRead(T0); // GPIO4
  Serial.println(t);

  static bool touched = false;

  if (t < 30 && !touched) {
    String event = "Touched at " + getCurrentTime();
    addToHistory(event);
    touched = true;
  } else if (t >= 30) {
    touched = false;
  }

  delay(200);
}
