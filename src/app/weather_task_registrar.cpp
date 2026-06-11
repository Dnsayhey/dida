#include "weather_task_registrar.h"

#include "board/board_config.h"
#include "system/task_manager.h"
#include "weather_data_sync.h"

void WeatherTaskRegistrar::registerTasks(const String& locationId,
                                         const String& latitude,
                                         const String& longitude,
                                         WeatherDataSync& weatherDataSync) {
  TaskManager::getInstance().addTask(
      [locationId, &weatherDataSync]() {
        weatherDataSync.syncCurrentConditions(locationId);
      },
      BoardConfig::WEATHER_NOW_INTERVAL_MS, "WeatherNowTask", true);
  TaskManager::getInstance().addTask(
      [latitude, longitude, &weatherDataSync]() {
        weatherDataSync.syncAirQualityNow(latitude, longitude);
      },
      BoardConfig::AIR_QUALITY_NOW_INTERVAL_MS, "AirQualityNowTask", true);
  TaskManager::getInstance().addTask(
      [locationId, &weatherDataSync]() {
        weatherDataSync.syncDailyForecast(locationId);
      },
      BoardConfig::WEATHER_7D_INTERVAL_MS, "Weather7DaysTask", true);
  TaskManager::getInstance().addTask(
      []() { TaskManager::getInstance().printTaskStatus(); },
      BoardConfig::TASK_LOG_INTERVAL_MS, "PrintTask", false);
  TaskManager::getInstance().addTask(
      []() {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
          Serial.println("Failed to get time");
          return;
        }
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      },
      BoardConfig::TIME_LOG_INTERVAL_MS, "PrintTime", false);
}
