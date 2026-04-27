#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <Arduino.h>
#include <lvgl.h>

namespace BoardConfig {

constexpr int TFT_HOR_RES = 240;
constexpr int TFT_VER_RES = 320;
constexpr lv_display_rotation_t TFT_ROTATION = LV_DISPLAY_ROTATION_0;

constexpr uint8_t DEFAULT_BUTTON_PIN = 8;
constexpr uint8_t DEFAULT_BACKLIGHT_PIN = 6;
constexpr uint8_t DEFAULT_LIGHT_SENSOR_PIN = 0;

constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 10000;
constexpr const char* PROVISIONING_AP_PASSWORD = "12345678";
constexpr unsigned long BACKLIGHT_INTERVAL_MS = 3000;
constexpr unsigned long NTP_SYNC_INTERVAL_MS = 1000UL * 60 * 60;
constexpr unsigned long WEATHER_NOW_INTERVAL_MS = 1000UL * 60 * 60 * 2;
constexpr unsigned long AIR_QUALITY_NOW_INTERVAL_MS = 1000UL * 60 * 60 * 2;
constexpr unsigned long WEATHER_7D_INTERVAL_MS = 1000UL * 60 * 60 * 12;
constexpr unsigned long TASK_LOG_INTERVAL_MS = 1000UL * 60 * 5;
constexpr unsigned long TIME_LOG_INTERVAL_MS = 1000UL * 60 * 5;

}  // namespace BoardConfig

#endif  // BOARD_CONFIG_H
