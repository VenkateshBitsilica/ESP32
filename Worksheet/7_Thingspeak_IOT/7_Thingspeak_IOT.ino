#include <WiFi.h>
#include "WiFiClient.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

const char* ssid = "Venky";
const char* password = "11223344";
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long ChannelNumber = 2982917;
const char * WriteAPIKey = "BRJE1GXSM5UK5UNL";

int number = 0;

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
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  int touch = touchRead(T0);
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(ChannelNumber, 1, touch, WriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
    
  delay(5000);
}
