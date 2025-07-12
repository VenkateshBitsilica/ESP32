void setup() {
  
  Serial.begin(115200);
  pinMode(2, INPUT);
  
}

void loop() {
  
  uint8_t state = digitalRead(2);
  if(state == 1)
    Serial.println("Button Pressed");
  else
    Serial.println("Button Released");
  delay(500);

}
