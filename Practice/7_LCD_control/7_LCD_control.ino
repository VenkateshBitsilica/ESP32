#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, World!");
  lcd.setCursor(0,1);
  lcd.print("Welcome");
}

void loop() {
      for(int i=0; i<10; i++)
      {
        lcd.setCursor(8,1);
        lcd.print(" ");
        lcd.print(i);
        delay(500);
      }
}
