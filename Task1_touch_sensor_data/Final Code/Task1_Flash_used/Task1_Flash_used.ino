#include <WiFi.h>       
#include <WebServer.h>  
#include <time.h>       
#include <SPIFFS.h>   //Serial Peripheral Interface Flash File System  

// WiFi credentials
const char* ssid = "Venky";
const char* password = "11223344";

// Create a web server object on port 80 (HTTP default port)
WebServer server(80);
// Syntax : WebServer server(port);

// Maximum history entries to store and display
#define MAX_HISTORY 10

// Buffer to store sensor touch history
String history[MAX_HISTORY];  // Circular buffer to store messages
int historyIndex = 0;         // Tracks the next write position in buffer

/*
 * This functions Writes the latest sensor touch history to the file stored in flash memory
 */
void write_history_to_File() {
  File file = SPIFFS.open("/Sensor_status.txt", FILE_WRITE); // Open file in write mode
  if (file) {
    // Write all non-empty history entries to file
    for (int i = 0; i < MAX_HISTORY; i++) {
      int index = (historyIndex + i) % MAX_HISTORY;
      if (history[index] != "") {
        file.println(history[index]);  // Write each entry to file (line by line)
      }
    }
    file.close();  
  } else {
    Serial.println("Failed to open file for writing");
  }
}

/*
 * This function adds new sensor status string to the buffer and updates the file
 */
void add_to_history(String msg) {
  if (historyIndex >= MAX_HISTORY) historyIndex = 0;  
  history[historyIndex++] = msg;                      // Add new message to buffer
  write_history_to_File();                            // calling this function to Save buffer to file in Flash
}

/* 
 * This function reads the previously stored sensor touch history from the flash file into the buffer \
  and return it to webserver to show the previous history before reading the new touvh sensor data
 */
void read_History_From_File() {
  File file = SPIFFS.open("/Sensor_status.txt", FILE_READ);  // Open file in read mode
  if (!file) {
    Serial.println("No log file found.");
    return;
  }

  historyIndex = 0; 
  while (file.available() && historyIndex < MAX_HISTORY) {
    history[historyIndex++] = file.readStringUntil('\n');  // Read each line until newline
  }

  file.close();
}

/* 
 * Purpose: Fetches the current date and time from NTP(Network Time Protocol) server
 * Syntaxes:
 *    struct tm timeinfo;
 *    getLocalTime(&timeinfo);
 *    strftime(buffer, sizeof(buffer), "format", &timeinfo);
 */
String getCurrentTime() {
  struct tm timeinfo;  // Structure to hold time
  if (!getLocalTime(&timeinfo)) {  // Get time from NTP
    return "Time not set";
  }
  char timeStr[30];  // Buffer to store formatted time string
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);  // this format the time & date to string format
  return String(timeStr);  // Return time as String
}

/* 
 * This function Creates a web page that displays the touch sensor history
 * Syntax:
 *    server.send(status_code, "content_type", content);
 */
void Server_page() {
  String html = "<html><head><title>ESP32 Touch Sensor</title>";
  html += "<meta http-equiv='refresh' content='2'>";  // Auto-refresh page every 2 seconds
  html += "</head><body>";
  html += "<h2>Touch History:</h2><ul>";

  // Display all history entries in HTML list
  for (int i = 0; i < MAX_HISTORY; i++) {
    int index = (historyIndex + i) % MAX_HISTORY;  // Circular buffer logic
    if (history[index] != "") {
      html += "<li>" + history[index] + "</li>";
    }
  }

  html += "</ul></body></html>";
  server.send(200, "text/html", html);  // Send HTML page to client
}

/* Setup function runs once when the ESP32 starts */
void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud

  // Initialize SPIFFS (Flash file system)
  if (!SPIFFS.begin(true)) {  // true = Format automatically if SPIFFS fails to mount
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {  // cheking wifi status wait until wifi got connected
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected! IP address:");
  Serial.println(WiFi.localIP());  // Print the IP address assigned to ESP32

  // Configure NTP time 
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov"); //configTime(timezone_offset, daylight_offset, "ntp_server1", "ntp_server2");

  read_History_From_File();  // Load any existing history from file

  server.on("/", Server_page);  // Assign the web page handler to root URL
  server.begin();               // Start the HTTP server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // Handle incoming HTTP client requests

  uint8_t t = touchRead(T0);  // Read touch sensor value from GPIO4 (T0)
  Serial.println(t);          // Print touch sensor reading

  static bool flag = false;  // Used to detect edge trigger (new touch event)

  // If sensor is touched (threshold < 20) and it was not touched before
  if (t < 20 && !flag) {
    String str = "Touched at " + getCurrentTime();  // Create string with timestamp
    add_to_history(str);  // Add event to history buffer and update file
    flag = true;         
  } else if (t >= 30) {     // If touch is released
    flag = false;        
  }

  delay(200);  
}

