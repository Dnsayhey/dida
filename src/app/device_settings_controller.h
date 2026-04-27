#ifndef DEVICE_SETTINGS_CONTROLLER_H
#define DEVICE_SETTINGS_CONTROLLER_H

#include <Arduino.h>

class DeviceSettingsController {
 public:
  String getBrightnessModeText() const;
  bool isDarkTheme() const;
  void switchToNextBrightnessMode();
  bool toggleTheme();
};

#endif  // DEVICE_SETTINGS_CONTROLLER_H
