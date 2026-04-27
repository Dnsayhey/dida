#include "app_runtime.h"

#include <Arduino.h>

#include "app_controller.h"
#include "board/board_config.h"
#include "system/task_manager.h"

AppRuntime& AppRuntime::getInstance() {
  static AppRuntime instance;
  return instance;
}

AppRuntime::AppRuntime() : _pageManager(PageManager::getInstance()) {}

bool AppRuntime::begin() {
  AppController::getInstance().begin();

#ifdef DEV_MODE
  DeviceConfigProvider().applyDevDefaults();
#endif

  return _displayDevice.begin();
}

void AppRuntime::runUiTaskLoop() {
  Serial.println("lvgl_task started");

  _pageManager.init();
  setupButtonInput();
  setupBacklight();

  Serial.println("lvgl_task loop started");

  while (true) {
    if (_displayDevice.lockForMs(100)) {
      _displayDevice.handleTimers();
      _buttonInput.update();
      _displayDevice.unlock();
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void AppRuntime::runDataTaskLoop() {
  Serial.println("init_and_api_task started");

  while (!_pageManager.isInitialized()) {
    vTaskDelay(pdMS_TO_TICKS(20));
  }

  _startupFlow.run(_displayDevice, _pageManager);

  Serial.println("init_and_api_task loop started");

  while (true) {
    TaskManager::getInstance().run();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void AppRuntime::dispatchButtonEvent(ButtonEvent event) {
  AppController& appController = AppController::getInstance();
  AppCommand command =
      appController.handleButtonEvent(event, _pageManager.getCurrentPage());
  _commandExecutor.execute(command, _pageManager);
}

void AppRuntime::setupButtonInput() {
  _buttonInput.begin(BoardConfig::DEFAULT_BUTTON_PIN,
                     [this](ButtonEvent event) { dispatchButtonEvent(event); });
}

void AppRuntime::setupBacklight() {
  _adaptiveBacklight.begin(BoardConfig::DEFAULT_BACKLIGHT_PIN,
                           BoardConfig::DEFAULT_LIGHT_SENSOR_PIN,
                           _deviceSettingsStore.getBacklightMode());
  _adaptiveBacklight.registerTasks();
}
