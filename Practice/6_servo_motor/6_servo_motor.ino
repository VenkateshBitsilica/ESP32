#include <ESP32Servo.h>

const int servoPin = 18; //Pin taken for generating PWM signals

Servo servo;
int pos = 0;

void setup() {
  servo.attach(servoPin)//, 500, 2400);
}

void loop() {
  for (pos = 0; pos <= 360; pos++) {
    servo.write(pos);
    delay(50);
  }
  delay(500);
  for (pos = 360; pos >= 0; pos--) {
    servo.write(pos);
    delay(50);
  }
}