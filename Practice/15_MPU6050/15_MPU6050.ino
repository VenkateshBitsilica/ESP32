#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
}

void loop() {

    int16_t ax,ay,az;
    int16_t gx,gy,gz;

    mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
    Serial.print("ACCELERO : ");
    Serial.println(ax);
    Serial.print("GYRO : ");
    Serial.println(gx);
    delay(1000);
}