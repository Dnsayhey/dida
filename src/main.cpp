#include <Arduino.h>

#include "app/app_runtime.h"
#include "freertos/task.h"

namespace {

void lvgl_task(void* pvParameters) {
  AppRuntime::getInstance().runUiTaskLoop();
}

void init_and_api_task(void* pvParameters) {
  AppRuntime::getInstance().runDataTaskLoop();
}

}  // namespace

void setup() {
  Serial.begin(115200);
  while (!Serial && !Serial.available()) {
  }

  delay(2000);

  Serial.println("Setup...");

  if (!AppRuntime::getInstance().begin()) {
    return;
  }

  xTaskCreate(lvgl_task, "lvgl_task", 8192, NULL, 2, NULL);
  xTaskCreate(init_and_api_task, "init_and_api_task", 8192, NULL, 1, NULL);

  Serial.println("Setup done");
}

void loop() { vTaskDelay(pdMS_TO_TICKS(10)); }
