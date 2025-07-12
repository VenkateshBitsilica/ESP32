//for RGB led if COM is cathod - GND, Anode - GPIO
void setup() {
  Serial.begin(115200);
    pinMode(2, OUTPUT);
    pinMode(0, OUTPUT);
    pinMode(4, OUTPUT);

}

void loop() {
    digitalWrite(2, HIGH);
    digitalWrite(0, LOW);
    digitalWrite(4, LOW);
    delay(1000);
    digitalWrite(2, LOW);
    digitalWrite(0, HIGH);
    digitalWrite(4, LOW);
    delay(1000);
    digitalWrite(2, LOW);
    digitalWrite(0, LOW);
    digitalWrite(4, HIGH);
    delay(1000);
}

