#ifndef WEATHER_DATA_SYNC_H
#define WEATHER_DATA_SYNC_H

#include <Arduino.h>

#include "device_config_provider.h"
#include "weather_data_client.h"

class WeatherDataSync {
 public:
  void ensureLocation(const DeviceConfigSnapshot& configSnapshot,
                      DeviceConfigProvider& configProvider);
  WeatherSyncResult syncCurrentConditions(const String& locationId);
  WeatherSyncResult syncDailyForecast(const String& locationId);
  WeatherSyncResult syncAirQualityNow(const String& latitude,
                                      const String& longitude);

 private:
  WeatherDataClient _weatherDataClient;
};

#endif  // WEATHER_DATA_SYNC_H
