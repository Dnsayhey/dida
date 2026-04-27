#ifndef WEATHER_SERVICE_H
#define WEATHER_SERVICE_H

#include "weather/weather_type.h"

class WeatherService {
 public:
  static WeatherService& getInstance();

  WeatherSyncResult updateCityLookup(String city, String adm = "");
  WeatherSyncResult updateWeatherNow(String location);
  WeatherSyncResult updateWeather7Days(String location);

  CityLookup getCityLookup() { return _cityLookup; }
  WeatherNow getWeatherNow() { return _weatherNow; }
  WeatherDaily* getWeather7Days() { return _weather7Days; }
  WeatherSyncResult getWeatherNowSyncResult() const {
    return _weatherNowSyncResult;
  }
  WeatherSyncResult getWeather7DaysSyncResult() const {
    return _weather7DaysSyncResult;
  }

 private:
  WeatherService() = default;
  WeatherService(const WeatherService&) = delete;
  WeatherService& operator=(const WeatherService&) = delete;

  CityLookup _cityLookup;
  WeatherNow _weatherNow;
  WeatherDaily _weather7Days[7];
  WeatherSyncResult _cityLookupSyncResult;
  WeatherSyncResult _weatherNowSyncResult;
  WeatherSyncResult _weather7DaysSyncResult;

  bool parseCityLookupResp(String json, CityLookup& cityLookup);
  bool parseWeatherNowResp(String json, WeatherNow& weatherNow);
  bool parseWeather7DaysResp(String json, WeatherDaily weather7Days[]);
};

#endif
