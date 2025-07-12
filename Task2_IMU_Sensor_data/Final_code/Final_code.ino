
#include <Wire.h>
#include <L3G.h>
#include <LPS.h>
#include <LSM303.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

#define MAX_HISTORY     10

L3G gyro;
LPS baro_sensor;
LSM303 accel_sensor;

WebServer server(80);

//String sensor_history = "";
String history[MAX_HISTORY];
int buffer_index = 0;

char *ssid = "Venky";
char *pswd = "11223344";

bool flag = false;


void web_page() {
    File file = SPIFFS.open("/sensor_data.txt", "r");
    String page = "<html><head><title>ESP32 IMU Sensor Data</title>";
    page += "<meta http-equiv='refresh' content='2'>";
    //page += "<table border=\"1\">";
    //page += "<tr><th>Header 1</th><th>Header 2</th></tr>";
    //page += "<tr><td>Data 1</td><td>Data 2</td></tr>";
    page += "<style>table, th, td { border:1px solid black; border-collapse:collapse; padding:8px; }</style>";
    page += "</head><body>";
    page += "<h2>IMU Sensor Data</h2>";
    page += "<table><tr><th>Accelerometer</th><th>Gyroscope</th><th>Barometer</th></tr>";

    while (file.available()) {
        String line = file.readStringUntil('\n');
        page += line;
    }

    page += "</table></body></html>";
    file.close();

    server.send(200, "text/html", page);
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    int count =0;

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    File file = SPIFFS.open("/sensor_data.txt", "r");
    if (file) {
        buffer_index = 0;
        while (file.available() && buffer_index < MAX_HISTORY) {
            String line = file.readStringUntil('\n');
            history[buffer_index] = line + "\n";  // Add newline to preserve format
            buffer_index++;
        }
        file.close();
        buffer_index = buffer_index % MAX_HISTORY;  // Wrap index if needed
    }

    WiFi.begin(ssid, pswd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        count++;
        if(count > 10)
        {
            //WiFi.mode(WIFI_STA);
            // WiFi.disconnect();
            int n = WiFi.scanNetworks();
            if(n>0){
            Serial.print(n);
            Serial.println(" networks found");
            for (int i = 0; i < n; ++i) 
            {
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(WiFi.SSID(i));
                Serial.print(" (");
                Serial.print(WiFi.RSSI(i));
                delay(10);
            }
            }
            count =0;
        }
    }
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    if (!gyro.init()) {
        Serial.println("Failed to detect Gyro sensor");
        while (1);
    }
    if (!baro_sensor.init()) {
        Serial.println("Failed to detect barometer");
        while (1);
    }
    if (!accel_sensor.init()) {
        Serial.println("Failed to detect Accelerometer sensor");
        while (1);
    }

    gyro.enableDefault();
    baro_sensor.enableDefault();
    accel_sensor.enableDefault();

    server.on("/", web_page);
    server.begin();
}

void loop() {
    server.handleClient();
    uint8_t touch = touchRead(T0);

   /*     int a_x = (int)accel_sensor.a.x;
        int a_y = (int)accel_sensor.a.y;
        int a_z = (int)accel_sensor.a.z;
        String a_x1 = String(a_x);
        String a_y1 = String(a_y);
        String a_z1 = String(a_z);
        int g_x = (int)gyro.g.x;
        int g_y = (int)gyro.g.y;
        int g_z = (int)gyro.g.z;
        String g_x1 = String(g_x);
        String g_y1 = String(g_y);
        String g_z1 = String(g_z);
        
        Serial.print("X : ");
        Serial.println((int)gyro.g.x);
        Serial.print("Y : ");
        Serial.println((int)gyro.g.y);
        Serial.print("Z : ");
        Serial.println((int)gyro.g.z);

        Serial.print("Barometer Reading :");
        Serial.println(pressure);

        Serial.print("X : ");
        Serial.println((int)accel_sensor.a.x);
        Serial.print("Y : ");
        Serial.println((int)accel_sensor.a.y);
        Serial.print("Z : ");
        Serial.println((int)accel_sensor.a.z
    */
    if (touch < 20 && !flag) {

        gyro.read();
        accel_sensor.read();
        int pressure = baro_sensor.readPressureMillibars();

        int a_x = (int)accel_sensor.a.x;
        int a_y = (int)accel_sensor.a.y;
        int a_z = (int)accel_sensor.a.z;

        int g_x = (int)gyro.g.x;
        int g_y = (int)gyro.g.y;
        int g_z = (int)gyro.g.z;

        String accel_data = "X: " + String(a_x) + " Y: " + String(a_y) + " Z: " + String(a_z);
        String Gyro_data = "X: " + String(g_x) + " Y: " + String(g_y) + " Z: " + String(g_z);
        String baro_data = "Pressure: " + String(pressure);

        Serial.println(accel_data);
        Serial.println(Gyro_data);
        Serial.println(baro_data);

        String table_row = "<tr><td>" + accel_data + "</td><td>" + Gyro_data + "</td><td>" + baro_data + "</td></tr>\n";

        // Store in circular buffer
        history[buffer_index] = table_row;
        buffer_index = (buffer_index + 1) % MAX_HISTORY;

        // Overwrite the file with the latest history
        File file = SPIFFS.open("/sensor_data.txt", "w");
        if (file) {
            for (int i = 0; i < MAX_HISTORY; i++) {
                int idx = (buffer_index + i) % MAX_HISTORY;
                if (history[idx] != "") {
                    file.print(history[idx]);
                }
            }

            file.close();
        }

        flag = true;
    } else {
        flag = false;
    }

    delay(200);
}

