#ifndef WEATHER_DATA_CLIENT_H
#define WEATHER_DATA_CLIENT_H

#include <Arduino.h>

#include "weather/weather_type.h"

class WeatherDataClient {
 public:
  CityLookup lookupLocation(const String& location, const String& adm);
  WeatherSyncResult updateCurrentConditions(const String& locationId);
  WeatherSyncResult updateDailyForecast(const String& locationId);
  WeatherSyncResult updateAirQualityNow(const String& latitude,
                                        const String& longitude);
  WeatherNow getCurrentConditions() const;
  WeatherDaily getDailyForecast(uint8_t dayOffset) const;
  AirQualityNow getAirQualityNow() const;
  WeatherSyncResult getCurrentConditionsSyncResult() const;
  WeatherSyncResult getDailyForecastSyncResult() const;
  WeatherSyncResult getAirQualityNowSyncResult() const;
};

#endif  // WEATHER_DATA_CLIENT_H
