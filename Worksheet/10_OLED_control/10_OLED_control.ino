#include <Wire.h>       //library to handle I2C communication
#include <U8g2lib.h>    //U8g2 graphics library supports OLED and LCD display's
int counter =0;
/* For SH1107 128x128 OLED via I2C on ESP32
   this crates u8g2 object to work with SH1107 OLED
   U8G2_SH1107_128X128_1_SW_I2C u8g2(rotation, clock pin, Data pin, Reset pin)
   _1_, _2_ refer to Page buffer mode, _F_ refer to Full buffer mode 
   U8G2_SH1107_128X128_1_SW_I2C u8g2(U8G2_R0, 22,  21, U8X8_PIN_NONE); 
   If i use SW interaction that makes delay to display text 
*/
U8G2_SH1107_128X128_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);  //HW interaction is more faster

void setup() {
  Serial.begin(115200);
  u8g2.begin();   //Initialization for OLED
  //u8g2_font_6x10_tr --> Font lists(https://github.com/olikraus/u8g2/wiki/fntlistall)
  u8g2.setFont(u8g2_font_ncenB08_tr);
  //u8g2.setFontMode(1);
  u8g2.clearBuffer();
  //u8g2.setCursor(0,16);
  // u8g2.print("OLED Control");
  // u8g2.sendBuffer();  //this will push the print msg to OLED
}

void loop() {
  /*Used in Page buffer mode where memory contraints matters*/
  // u8g2.firstPage();
  // do {
  //   u8g2.drawStr(10, 30, "Hello, OLED!");
  //   u8g2.drawStr(10, 50, "128x128 Display");
  // } while (u8g2.nextPage());
  u8g2.clearBuffer();               // Clear internal memory
  u8g2.setCursor(10, 20);           
  u8g2.print("OLED Control");

  u8g2.setCursor(10, 60);           // Counter line
  u8g2.print("Counter: ");
  u8g2.print(counter);

  u8g2.sendBuffer();                // Push to screen

  counter = (counter + 1) % 10;     // 0 to 9 cycling
  delay(1000);                      // Update every second

}
