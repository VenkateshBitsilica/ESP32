#include "DHT.h"
DHT dht2(2,DHT22);
void setup() {
  
  Serial.begin(115200);
  
}

void loop() {
    Serial.println("Temperature");
    Serial.println((dht2.readTemperature()));
    Serial.println("Humidity");
    Serial.println((dht2.readHumidity()));
    delay(500);

}
