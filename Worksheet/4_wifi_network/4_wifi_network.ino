#include <WiFi.h>

char *ssid = "Venky";
char *pswd = "11223344";

void setup() {
 Serial.begin(115200);
 WiFi.begin(ssid,pswd);

 if(WiFi.status() != WL_CONNECTED)
 {
    Serial.print(".");
    delay(500);
 }

  Serial.print("Connected with IP Address : ");
 Serial.println(WiFi.localIP());
//  WiFi.mode(WIFI_STA);
//  WiFi.disconnect();
//  delay(100);

//  Serial.println("Setup completed");

}

void loop() {
  // put your main code here, to run repeatedly:
/*  int n = WiFi.scanNetworks();
  if(n==0)
  {
    Serial.println("No networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(")");
      delay(10);
    }
  }


  delay(10000);
  */
}
