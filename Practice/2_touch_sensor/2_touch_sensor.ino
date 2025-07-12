void setup() {
  
  Serial.begin(115200);
}

void loop() {
  uint8_t t = touchRead(T0);
    Serial.println(t); //GPIO4 - TOUCH 0
    delay(500);
}