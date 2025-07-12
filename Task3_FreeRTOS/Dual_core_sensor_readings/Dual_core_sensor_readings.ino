#include "DHT.h"

#define LED1    2
#define LED2    12
//#define TOUCH   4
#define MOTION  14
#define TEMP    5

TaskHandle_t led_blink_1_handle;
TaskHandle_t led_blink_2_hanlde;
TaskHandle_t motion_handle;
TaskHandle_t dht_handle;

BaseType_t task_check;

DHT dht(TEMP,DHT22);

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(MOTION, INPUT);

 task_check = xTaskCreatePinnedToCore(blink_1, "Blinking_LED_1", 1000, NULL, 1, &led_blink_1_handle,0);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for blink_1");
  task_check = xTaskCreatePinnedToCore(Motion_sensor, "Motion_sensor_reading", 1000, NULL, 1, &motion_handle,0);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for Motion_sensor");
  task_check = xTaskCreatePinnedToCore(blink_2, "Blinked_LED_2", 1000, NULL, 1, &led_blink_2_hanlde,1);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for blink_2");
  task_check = xTaskCreatePinnedToCore(temp_sensor, "Temp_Humidity_Readings", 1000, NULL, 1, &dht_handle,1);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for temp_sensor");
}

void blink_1(void *p)
{
  while(1)
  {
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    delay(500);
  }
}

void blink_2(void *p)
{
  while(1)
  {
    digitalWrite(LED2, HIGH);
    delay(300);
    digitalWrite(LED2, LOW);
    delay(300);
  }
}

void Motion_sensor(void *p)
{
  while(1)
  {
    Serial.print("Motion Sensor Readings : ");
    Serial.println(digitalRead(MOTION));
    delay(500);
  }
}
void temp_sensor(void *p)
{
  while(1)
  {
    Serial.print("Temperature :");
    Serial.println(dht.readTemperature());
    delay(500);
  }
}

void loop() {

}
