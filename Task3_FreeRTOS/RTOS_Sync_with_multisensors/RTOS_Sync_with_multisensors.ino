#include "DHT.h"

#define LED1        2
#define LED2        12
#define TOUCH       4
#define GAS         14
#define GAS_ANALOG  13
#define TEMP        5

TaskHandle_t led_blink_1_handle;
TaskHandle_t led_blink_2_hanlde;
TaskHandle_t gas_handle;
TaskHandle_t dht_handle;

//Create Semaphore handle
SemaphoreHandle_t sem;
M
BaseType_t task_check;

void IRAM_ATTR gas_isr()
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  //xSemaphoreGiveFromISR(sem,NULL)//&xHigherPriorityTaskWoken);
    vTaskNotifyGiveFromISR(gas_handle,&xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) {
      portYIELD_FROM_ISR();
  }
}

DHT dht(TEMP,DHT22);

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(GAS, INPUT);

  sem = xSemaphoreCreateBinary();
  
/*
ESP32 defalt it will run on Core 1 when task is created with xTaskCreate();
For specifying cores we have to use xTaskCreatePinnedToCore() by mentioning core number 0 or 1 as last patrameter
*/
 task_check = xTaskCreatePinnedToCore(blink_1, "Blinking_LED_1", 1000, NULL, 1, &led_blink_1_handle,0);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for blink_1");
  task_check = xTaskCreatePinnedToCore(Gas_sensor, "Gas_sensor_reading", 1000, NULL, 1, &gas_handle,0);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for Gas_sensor");
  task_check = xTaskCreatePinnedToCore(blink_2, "Blinked_LED_2", 1000, NULL, 1, &led_blink_2_hanlde,1);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for blink_2");
  task_check = xTaskCreatePinnedToCore(temp_sensor, "Temp_Humidity_Readings", 1000, NULL, 1, &dht_handle,1);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for temp_sensor");

//This will enable and detect the ISR's when an external Interrupt is given
  attachInterrupt(digitalPinToInterrupt(GAS),gas_isr, CHANGE);

}

/*Task1 for blinking LED 1*/
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

/*Task2 for Blinking LED 2*/
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

/*Task for reasding MQ2 sensor data*/
void Gas_sensor(void *p)
{
  while(1)
  {
    if(ulTaskNotifyTake(pdTRUE,portMAX_DELAY) == 1){
    Serial.print("Gas Sensor Readings : ");
    Serial.println(analogRead(GAS_ANALOG));
    }
    
    delay(10);
  }
}

/*Task for reading DHT22 sensor data*/
void temp_sensor(void *p)
{
  while(1)
  {
    Serial.print("Temperature :");
    Serial.println(dht.readTemperature());
    Serial.print("Humidity :");
    Serial.println(dht.readHumidity());
    delay(500);
  }
}

void loop() {

}
