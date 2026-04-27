#ifndef DEVICE_SETTINGS_STORE_H
#define DEVICE_SETTINGS_STORE_H

#include "hardware/backlight.h"

class DeviceSettingsStore {
 public:
  BrightnessMode getBacklightMode() const;
  void setBacklightMode(BrightnessMode mode);
  bool getDarkTheme() const;
  void setDarkTheme(bool enabled);
};

#endif  // DEVICE_SETTINGS_STORE_H
