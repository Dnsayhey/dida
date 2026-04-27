#include "weather_data_client.h"

#include "weather/weather_service.h"

namespace {

constexpr uint8_t kWeatherDailyCount = 7;

}

String WeatherDataClient::lookupLocationId(const String& location,
                                           const String& adm) {
  WeatherService::getInstance().updateCityLookup(location.c_str(), adm.c_str());
  return WeatherService::getInstance().getCityLookup().id;
}

WeatherSyncResult WeatherDataClient::updateCurrentConditions(
    const String& locationId) {
  return WeatherService::getInstance().updateWeatherNow(locationId);
}

WeatherSyncResult WeatherDataClient::updateDailyForecast(
    const String& locationId) {
  return WeatherService::getInstance().updateWeather7Days(locationId);
}

WeatherNow WeatherDataClient::getCurrentConditions() const {
  return WeatherService::getInstance().getWeatherNow();
}

WeatherDaily WeatherDataClient::getDailyForecast(uint8_t dayOffset) const {
  if (dayOffset >= kWeatherDailyCount) {
    return WeatherDaily{};
  }

  return WeatherService::getInstance().getWeather7Days()[dayOffset];
}

WeatherSyncResult WeatherDataClient::getCurrentConditionsSyncResult() const {
  return WeatherService::getInstance().getWeatherNowSyncResult();
}

WeatherSyncResult WeatherDataClient::getDailyForecastSyncResult() const {
  return WeatherService::getInstance().getWeather7DaysSyncResult();
}
