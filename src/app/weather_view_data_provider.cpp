#include "weather_view_data_provider.h"

namespace {

const char* kCurrentWeatherSyncingText = "天气同步中";
const char* kCurrentWeatherFailedText = "天气同步失败";
const char* kDailyForecastSyncingText = "预报同步中";
const char* kDailyForecastFailedText = "预报同步失败";

bool hasCurrentConditions(const WeatherNow& weatherNow) {
  return !weatherNow.text.isEmpty() && !weatherNow.temp.isEmpty();
}

bool hasDailyForecast(const WeatherDaily& weather) {
  return !weather.fxDate.isEmpty() && !weather.textDay.isEmpty();
}

String formatCurrentConditions(const WeatherNow& weatherNow) {
  return weatherNow.text + " " + weatherNow.temp + "℃" + "\n" +
         weatherNow.windDir + " " + weatherNow.windScale + "级";
}

String formatDailyForecast(const WeatherDaily& weather) {
  return weather.fxDate.substring(5, 10) + " " + weather.textDay + " " +
         weather.tempMin + "/" + weather.tempMax + "℃";
}

String getCurrentWeatherStatusText(const WeatherSyncResult& syncResult) {
  switch (syncResult.status) {
    case WeatherSyncStatus::Failed:
      return kCurrentWeatherFailedText;
    case WeatherSyncStatus::Ready:
      return "";
    case WeatherSyncStatus::Idle:
    case WeatherSyncStatus::Syncing:
    default:
      return kCurrentWeatherSyncingText;
  }
}

String getDailyForecastStatusText(const WeatherSyncResult& syncResult) {
  switch (syncResult.status) {
    case WeatherSyncStatus::Failed:
      return kDailyForecastFailedText;
    case WeatherSyncStatus::Ready:
      return "";
    case WeatherSyncStatus::Idle:
    case WeatherSyncStatus::Syncing:
    default:
      return kDailyForecastSyncingText;
  }
}

}  // namespace

CurrentWeatherViewData WeatherViewDataProvider::getCurrentWeatherViewData()
    const {
  WeatherNow weatherNow = _weatherDataClient.getCurrentConditions();
  WeatherSyncResult syncResult =
      _weatherDataClient.getCurrentConditionsSyncResult();
  bool hasData = hasCurrentConditions(weatherNow);
  String statusText = getCurrentWeatherStatusText(syncResult);
  String conditionsText =
      hasData ? formatCurrentConditions(weatherNow) : statusText;

  return {_configProvider.getLocationName(), conditionsText, hasData,
          statusText};
}

DailyForecastViewData WeatherViewDataProvider::getDailyForecastViewData(
    uint8_t dayOffset) const {
  WeatherDaily weather = _weatherDataClient.getDailyForecast(dayOffset);
  WeatherSyncResult syncResult = _weatherDataClient.getDailyForecastSyncResult();
  bool hasData = hasDailyForecast(weather);
  String statusText = getDailyForecastStatusText(syncResult);
  String forecastText = hasData ? formatDailyForecast(weather) : statusText;

  return {forecastText, hasData, statusText};
}
