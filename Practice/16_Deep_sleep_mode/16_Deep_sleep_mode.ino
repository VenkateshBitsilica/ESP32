#include <esp_sleep.h>
#include <esp_log.h>

// Define the sleep time in seconds
#define SLEEP_DURATION 10

// Define the time factor (microseconds to seconds)
#define uS_TO_S_FACTOR 1000000ULL

// Variable to store the boot count, stored in RTC memory
RTC_DATA_ATTR int bootCount = 0;

void setup() {
  Serial.begin(115200);
  // Increment boot count on each wakeup
  bootCount++;
  Serial.println("Boot number: " + String(bootCount));

  // Configure the wakeup source (timer)
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for " + String(SLEEP_DURATION) + " Seconds");

  // Put the ESP32 into deep sleep
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}

void loop() {
  // This code will not be executed in deep sleep
}