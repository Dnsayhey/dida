#include "device_settings_store.h"

#include "config/config_manager.h"

namespace {

constexpr int kFirstBrightnessMode = static_cast<int>(AUTO);
constexpr int kLastBrightnessMode = static_cast<int>(LEVEL_3);

BrightnessMode normalizeBrightnessMode(int mode) {
  if (mode < kFirstBrightnessMode || mode > kLastBrightnessMode) {
    return AUTO;
  }

  return static_cast<BrightnessMode>(mode);
}

}  // namespace

BrightnessMode DeviceSettingsStore::getBacklightMode() const {
  return normalizeBrightnessMode(
      ConfigManager::getInstance().getBacklightMode());
}

void DeviceSettingsStore::setBacklightMode(BrightnessMode mode) {
  ConfigManager::getInstance().setBacklightMode(static_cast<int>(mode));
}

bool DeviceSettingsStore::getDarkTheme() const {
  return ConfigManager::getInstance().getThemeMode() == 1;
}

void DeviceSettingsStore::setDarkTheme(bool enabled) {
  ConfigManager::getInstance().setThemeMode(enabled ? 1 : 0);
}
