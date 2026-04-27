#include "ui_theme.h"

#include "app/device_settings_controller.h"

UiThemeColors UiTheme::current() {
  return DeviceSettingsController().isDarkTheme() ? dark() : light();
}

UiThemeColors UiTheme::light() {
  return {
      lv_color_hex(0xF4EFE5), lv_color_hex(0xFFFAF0),
      lv_color_hex(0x241B14), lv_color_hex(0xDFD0BD),
      LV_OPA_COVER,           LV_OPA_80,
      LV_OPA_40,
  };
}

UiThemeColors UiTheme::dark() {
  return {
      lv_color_hex(0x090D12), lv_color_hex(0x18202A),
      lv_color_hex(0xF2EBDD), lv_color_hex(0x3E4A57),
      LV_OPA_COVER,           LV_OPA_90,
      LV_OPA_60,
  };
}
