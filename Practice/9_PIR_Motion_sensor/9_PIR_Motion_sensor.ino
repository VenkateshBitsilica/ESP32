
void setup() {
  Serial.begin(115200);
    pinMode(2, INPUT);
}

void loop() {
    Serial.println(digitalRead(2));
    delay(500);
}