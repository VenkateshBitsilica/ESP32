void setup() {
  
  Serial.begin(115200);
  
}

void loop() {
  int potentimeter_val = analogRead(2);
  Serial.println(potentimeter_val);
  delay(500);
}