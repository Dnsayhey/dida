#ifndef APP_RUNTIME_H
#define APP_RUNTIME_H

#include "app_command_executor.h"
#include "device_settings_store.h"
#include "hardware/adaptive_backlight.h"
#include "hardware/button_input.h"
#include "hardware/display_device.h"
#include "pages/page_manager.h"
#include "startup_flow.h"

class AppRuntime {
 public:
  static AppRuntime& getInstance();

  bool begin();
  void runUiTaskLoop();
  void runDataTaskLoop();

 private:
  AppRuntime();
  AppRuntime(const AppRuntime&) = delete;
  AppRuntime& operator=(const AppRuntime&) = delete;

  void dispatchButtonEvent(ButtonEvent event);
  void setupButtonInput();
  void setupBacklight();

  PageManager& _pageManager;
  ButtonInput _buttonInput;
  DisplayDevice _displayDevice;
  AdaptiveBacklight _adaptiveBacklight;
  DeviceSettingsStore _deviceSettingsStore;
  AppCommandExecutor _commandExecutor;
  StartupFlow _startupFlow;
};

#endif  // APP_RUNTIME_H
