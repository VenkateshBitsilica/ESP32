#define <Wire.h>

#define GYRO_ADDRESS 0x6B  // Update if your SA0 pin is LOW

// #define I2C_SCL   22
// #define I2C_SDA   21

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

int16_t read16(uint8_t reg) {
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.write(reg | 0x80);  // Set auto-increment
  Wire.endTransmission(false);
  Wire.requestFrom(GYRO_ADDRESS, 2);

  int16_t low = Wire.read();
  int16_t high = Wire.read();

  return (high << 8) | low;
}

void readGyroData(int16_t &x, int16_t &y, int16_t &z) {
  x = read16(0x28);
  y = read16(0x2A);
  z = read16(0x2C);
}
void setup() {
  Serial.begin(115200);
  Wire.begin();
  /*
  Gyro sensor setup
  0x20 is control register 1
  0x23 is control register 4
  */
  writeRegister(0x20, 0b00001111); // Enable all axes, 95 Hz ODR
  writeRegister(0x23, 0b00110000); // Full scale ±2000 dps
}

void loop() {
  int16_t x, y, z;
  readGyroData(x, y, z);

  Serial.print("Gyro X: ");
  Serial.print(x);
  Serial.print("Gyro Y: ");
  Serial.print(y);
  Serial.print("Gyro Z: "); 
  Serial.println(z);

  delay(500);
}


