#include "adaptive_backlight.h"

#include "hardware/backlight.h"
#include "board/board_config.h"
#include "hardware/light_sensor.h"
#include "system/task_manager.h"

void AdaptiveBacklight::begin(uint8_t backlightPin, uint8_t lightSensorPin,
                              BrightnessMode initialMode) {
  LightSensor::getInstance().begin(lightSensorPin);
  Backlight::getInstance().begin(backlightPin, initialMode);
}

void AdaptiveBacklight::registerTasks() {
  TaskManager::getInstance().addTask(
      [this]() { update(); },
      BoardConfig::BACKLIGHT_INTERVAL_MS, "BacklightTask", true);
}

void AdaptiveBacklight::update() {
  Backlight::getInstance().setBrightness(
      LightSensor::getInstance().readBrightness());
  Backlight::getInstance().update();
}
