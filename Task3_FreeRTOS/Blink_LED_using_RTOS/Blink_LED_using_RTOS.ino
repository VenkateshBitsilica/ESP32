#define LED1    2
#define LED2    4

TaskHandle_t led_blink_1;
TaskHandle_t led_blink_2;

BaseType_t task_check;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

 task_check = xTaskCreate(blink_1, "Blinking_LED_1", 200, NULL, 1, &led_blink_1);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for blink_1");
  task_check = xTaskCreate(blink_2, "Blinked_LED_2", 200, NULL, 1, &led_task_2);
  if(task_check != pdPASS)
      Serial.println("Task creation failed for blink_2");

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

void blink_1(void *p)
{
  while(1)
  {
    digitalWrite(LED2, HIGH);
    delay(300);
    digitalWrite(LED2, LOW);
    delay(300);
  }
}

void loop() {

}
