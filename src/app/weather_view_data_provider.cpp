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

bool hasAirQuality(const AirQualityNow& airQualityNow) {
  return !airQualityNow.aqi.isEmpty() || !airQualityNow.category.isEmpty();
}

String formatCurrentConditions(const WeatherNow& weatherNow) {
  return weatherNow.text + " " + weatherNow.temp + "℃" + "\n" +
         weatherNow.windDir + " " + weatherNow.windScale + "级";
}

String formatTemperature(const WeatherNow& weatherNow) {
  return weatherNow.temp.isEmpty() ? "--℃" : weatherNow.temp + "℃";
}

String formatFeelsLike(const WeatherNow& weatherNow) {
  return weatherNow.feelsLike.isEmpty() ? "体感 --℃"
                                        : "体感 " + weatherNow.feelsLike + "℃";
}

String formatHumidity(const WeatherNow& weatherNow) {
  return weatherNow.humidity.isEmpty() ? "湿度 --%"
                                       : "湿度 " + weatherNow.humidity + "%";
}

String formatWind(const WeatherNow& weatherNow) {
  if (weatherNow.windDir.isEmpty() && weatherNow.windScale.isEmpty()) {
    return "风力 --";
  }
  return weatherNow.windDir + " " + weatherNow.windScale + "级";
}

String formatVisibility(const WeatherNow& weatherNow) {
  return weatherNow.vis.isEmpty() ? "能见度 --km"
                                  : "能见度 " + weatherNow.vis + "km";
}

String formatAirQuality(const AirQualityNow& airQualityNow) {
  if (!hasAirQuality(airQualityNow)) {
    return "空气质量 --";
  }

  String text = "空气质量 ";
  if (!airQualityNow.aqi.isEmpty()) {
    text += airQualityNow.aqi;
  }
  if (!airQualityNow.category.isEmpty()) {
    text += " " + airQualityNow.category;
  }
  return text;
}

String formatPm2p5(const AirQualityNow& airQualityNow) {
  if (airQualityNow.pm2p5.isEmpty()) {
    return "PM2.5 --";
  }
  return "PM2.5 " + airQualityNow.pm2p5;
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
  AirQualityNow airQualityNow = _weatherDataClient.getAirQualityNow();
  WeatherSyncResult syncResult =
      _weatherDataClient.getCurrentConditionsSyncResult();
  bool hasData = hasCurrentConditions(weatherNow);
  bool hasAirQualityData = hasAirQuality(airQualityNow);
  String statusText = getCurrentWeatherStatusText(syncResult);
  String conditionsText =
      hasData ? formatCurrentConditions(weatherNow) : statusText;

  return {_configProvider.getLocationName(), conditionsText,
          weatherNow.text,
          formatTemperature(weatherNow),
          formatFeelsLike(weatherNow),
          formatHumidity(weatherNow),
          formatWind(weatherNow),
          formatVisibility(weatherNow),
          formatAirQuality(airQualityNow),
          formatPm2p5(airQualityNow),
          hasData,
          hasAirQualityData,
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
