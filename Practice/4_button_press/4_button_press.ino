void setup() {
  
  Serial.begin(115200);
  pinMode(2, INPUT);
  
}

void loop() {
  
  uint8_t state = digitalRead(2);
  Serial.println(state);
  delay(500);

}
