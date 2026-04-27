#ifndef WEATHER_SERVICE_H
#define WEATHER_SERVICE_H

#include "weather/weather_type.h"

class WeatherService {
 public:
  static WeatherService& getInstance();

  WeatherSyncResult updateCityLookup(String city, String adm = "");
  WeatherSyncResult updateWeatherNow(String location);
  WeatherSyncResult updateWeather7Days(String location);
  WeatherSyncResult updateAirQualityNow(const String& latitude,
                                        const String& longitude);

  CityLookup getCityLookup() { return _cityLookup; }
  WeatherNow getWeatherNow() { return _weatherNow; }
  WeatherDaily* getWeather7Days() { return _weather7Days; }
  AirQualityNow getAirQualityNow() { return _airQualityNow; }
  WeatherSyncResult getWeatherNowSyncResult() const {
    return _weatherNowSyncResult;
  }
  WeatherSyncResult getWeather7DaysSyncResult() const {
    return _weather7DaysSyncResult;
  }
  WeatherSyncResult getAirQualityNowSyncResult() const {
    return _airQualityNowSyncResult;
  }

 private:
  WeatherService() = default;
  WeatherService(const WeatherService&) = delete;
  WeatherService& operator=(const WeatherService&) = delete;

  CityLookup _cityLookup;
  WeatherNow _weatherNow;
  WeatherDaily _weather7Days[7];
  AirQualityNow _airQualityNow;
  WeatherSyncResult _cityLookupSyncResult;
  WeatherSyncResult _weatherNowSyncResult;
  WeatherSyncResult _weather7DaysSyncResult;
  WeatherSyncResult _airQualityNowSyncResult;

  bool parseCityLookupResp(String json, CityLookup& cityLookup);
  bool parseWeatherNowResp(String json, WeatherNow& weatherNow);
  bool parseWeather7DaysResp(String json, WeatherDaily weather7Days[]);
  bool parseAirQualityNowResp(String json, AirQualityNow& airQualityNow);
};

#endif
