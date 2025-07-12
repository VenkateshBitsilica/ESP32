#include <Wire.h>
#include <L3G.h>
#include <LPS.h>
#include <LSM303.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

L3G gyro;
LPS baro_sensor;
LSM303 accel_sensor;

WebServer server(80);

String accel_data = "";
String Gyro_data = "";
String baro_data = "";

char *ssid = "Venky";
char *pswd = "11223344";

void web_page()
{
    String page = "<html><head><title>ESP32 IMU Sensor Data</title>";
    page += "<meta http-equiv='refresh' content='2'>";
    page += "</head><body>";
    //Create a table for storing data 
    page += "<h2>Accelerometer Sensor Data :</h2>";
    page += "<ul>" + accel_data + "</ul>";
    page += "<h2>Gyro Sensor Data :</h2>";
    page += "<ul>" + Gyro_data + "</ul>";
    page += "<h2>Barometer Data :</h2>";
    page += "<ul>" + baro_data + "</ul>";
    page += "</body></html>";

    server.send(200, "text/html", page);
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    WiFi.begin(ssid, pswd);

    while(WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    if(!gyro.init())
    {
      Serial.println("Failed to detect Gyro sensor");
      while(1);
    }
    if(!baro_sensor.init())
    {
      Serial.println("Failed to detect barometer");
      while(1);
    }
    if(!accel_sensor.init())
    {
      Serial.println("Failed to detect Accelerometer sensor");
      while(1);     
    }

    gyro.enableDefault();
    baro_sensor.enableDefault();
    accel_sensor.enableDefault();

  server.on("/", web_page);  // Assign the web page handler to root URL
  server.begin();
}

void loop() {
    server.handleClient();
    gyro.read();
    accel_sensor.read();
    int pressure = baro_sensor.readPressureMillibars();
    uint8_t touch = touchRead(T0);
    bool flag = false;

    if(touch < 20 && !flag){
    
        int a_x = (int)accel_sensor.a.x;
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
        Serial.println((int)accel_sensor.a.z);

        accel_data = "X : " + a_x1 + " | Y : " + a_y1 + " | Z : " + a_z1;
        Gyro_data = "X : " + g_x1 + " | Y : " + g_y1 + " | Z : " + g_z1;
        baro_data = "Pressure : "+ String(pressure);

      flag = true;
    }
    else 
    {
      flag = false;
    }

    delay(200);

}
