#ifndef WEATHER_VIEW_DATA_PROVIDER_H
#define WEATHER_VIEW_DATA_PROVIDER_H

#include <Arduino.h>

#include "device_config_provider.h"
#include "weather_data_client.h"
#include "weather/weather_type.h"

struct CurrentWeatherViewData {
  String locationName;
  String conditionsText;
  bool hasData;
  String statusText;
};

struct DailyForecastViewData {
  String forecastText;
  bool hasData;
  String statusText;
};

class WeatherViewDataProvider {
 public:
  CurrentWeatherViewData getCurrentWeatherViewData() const;
  DailyForecastViewData getDailyForecastViewData(uint8_t dayOffset) const;

 private:
  DeviceConfigProvider _configProvider;
  WeatherDataClient _weatherDataClient;
};

#endif  // WEATHER_VIEW_DATA_PROVIDER_H
