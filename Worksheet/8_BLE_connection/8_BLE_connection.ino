#include "BluetoothSerial.h"

BluetoothSerial ble;
char ON = 'a';
char OFF = 'b';

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ble.begin("ESP32");
  //pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //char msg;
  int touch = touchRead(T0);
  Serial.println(touch);
  if(touch <30)
    ble.println("Touch detected");
/*  if(ble.available())
  {
    // msg = ble.read();
    // Serial.write(msg);
    if(touch < 30)
    {
      //digitalWrite(2,HIGH);
      //Serial.println("LED ON");
      //ble.println("LED ON Through Bluetooth message");
      //delay(3000);
    }
    // if(msg == OFF)
    // {
    //   digitalWrite(2,LOW);
    //   Serial.println("LED OFF");
    //   ble.println("LED OFF Through Bluetooth message");
    // }
  }*/
  delay(500);
}
