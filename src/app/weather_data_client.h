#ifndef WEATHER_DATA_CLIENT_H
#define WEATHER_DATA_CLIENT_H

#include <Arduino.h>

#include "weather/weather_type.h"

class WeatherDataClient {
 public:
  String lookupLocationId(const String& location, const String& adm);
  WeatherSyncResult updateCurrentConditions(const String& locationId);
  WeatherSyncResult updateDailyForecast(const String& locationId);
  WeatherNow getCurrentConditions() const;
  WeatherDaily getDailyForecast(uint8_t dayOffset) const;
  WeatherSyncResult getCurrentConditionsSyncResult() const;
  WeatherSyncResult getDailyForecastSyncResult() const;
};

#endif  // WEATHER_DATA_CLIENT_H
