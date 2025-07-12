
void setup() {
  
  Serial.begin(115200);
  //pinMode(2, INPUT); // for digital read 
  // for analog reading no need to set pin mode
  
}

void loop() {
  
  uint8_t state = analogRead(2);
  //uint8_t state = digitalRead(2); // for reading digital 0 or 1 1-high brightness, 0-dark

  Serial.println(state);
  delay(500);

}
