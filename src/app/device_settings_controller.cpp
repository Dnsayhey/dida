#include "device_settings_controller.h"

#include "backlight_controller.h"
#include "device_settings_store.h"

namespace {

String formatBrightnessMode(BrightnessMode mode) {
  switch (mode) {
    case AUTO:
      return "自动";
    case LEVEL_1:
      return "低";
    case LEVEL_2:
      return "中";
    case LEVEL_3:
      return "高";
    default:
      return "自动";
  }
}

}  // namespace

String DeviceSettingsController::getBrightnessModeText() const {
  return formatBrightnessMode(BacklightController().currentMode());
}

bool DeviceSettingsController::isDarkTheme() const {
  return DeviceSettingsStore().getDarkTheme();
}

void DeviceSettingsController::switchToNextBrightnessMode() {
  BrightnessMode nextMode = BacklightController().switchToNextMode();
  DeviceSettingsStore().setBacklightMode(nextMode);
}

bool DeviceSettingsController::toggleTheme() {
  bool nextTheme = !isDarkTheme();
  DeviceSettingsStore().setDarkTheme(nextTheme);
  return nextTheme;
}
