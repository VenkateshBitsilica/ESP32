#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  Serial.begin(115200);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, World!");
  lcd.setCursor(0,1);
  lcd.print("Welcome");
  
  delay(2000);
  lcd.clear(); // clearing the display 
}

void loop() {
  
  lcd.setCursor(0,0);
  int ver = analogRead(0);  // Reading vertical position
  int Hor = analogRead(4);  // Reading Horizontal position
  lcd.print("Veritical : ");
  lcd.print(ver);
  lcd.setCursor(0,1);
  lcd.print("Horizontal : ");
  lcd.print(Hor);
  delay(200);
}
