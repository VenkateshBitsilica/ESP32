#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <time.h>

const char* ssid = "Venky";
const char* password = "11223344";

WebServer server(80);

// Replace this with your company cloud URL
const char* cloudEndpoint = "http://cloud.company.com/upload";

#define MAX_HISTORY 10
String history[MAX_HISTORY];
int historyIndex = 0;

void addToHistory(String msg) {
  if (historyIndex >= MAX_HISTORY) historyIndex = 0;
  history[historyIndex++] = msg;
}

String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Time not set";
  char timeStr[30];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeStr);
}


void uploadToCloud(const String& event) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(cloudEndpoint);  // Company cloud API URL
    http.addHeader("Content-Type", "application/json");

    String json = "{\"sensor\": \"touch\", \"value\": \"" + event + "\"}";

    int httpCode = http.POST(json);

    if (httpCode > 0) {
      Serial.print("Cloud upload successful, code: ");
      Serial.println(httpCode);
    } else {
      Serial.print("Cloud upload failed, error: ");
      Serial.println(http.errorToString(httpCode));
    }

    http.end();
  } else {
    Serial.println("⚠️ Not connected to Wi-Fi");
  }
}

void handleRoot() {
  String html = "<html><head><title>ESP32 Touch Sensor</title>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "</head><body><h2>Touch History:</h2><ul>";

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
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n WiFi Connected. IP:");
  Serial.println(WiFi.localIP());

  configTime(19800, 0, "pool.ntp.org", "time.nist.gov"); // Set Indian time zone

  server.on("/", handleRoot);
  server.begin();
  Serial.println("🌐 Web server started");
}

void loop() {
  server.handleClient();

  uint8_t t = touchRead(T0); // GPIO 4
  Serial.println(t);

  static bool touched = false;

  if (t < 30 && !touched) {
    String timestamp = getCurrentTime();
    String event = "Touched at " + timestamp;
    addToHistory(event);
    uploadToCloud(event);  // ⬅️ Send to cloud here
    touched = true;
  } else if (t >= 30) {
    touched = false;
  }

  delay(200);
}
