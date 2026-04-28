#include "weather_service.h"

#include "ArduinoJson.h"
#include "ArduinoUZlib.h"
#include "net/request.h"
#include "weather/weather_type.h"
#include "weather_config.h"

#include <cstring>

namespace {

constexpr size_t kWeatherDailyCount = 7;

WeatherSyncResult syncingResult() {
  return {WeatherSyncStatus::Syncing, "", millis()};
}

WeatherSyncResult readyResult() {
  return {WeatherSyncStatus::Ready, "", millis()};
}

WeatherSyncResult failedResult(const String& message) {
  return {WeatherSyncStatus::Failed, message, millis()};
}

bool isApiCodeOk(JsonDocument& doc) {
  return doc["code"] == "200";
}

String apiCodeMessage(JsonDocument& doc) {
  return "API code: " + doc["code"].as<String>();
}

bool isWeatherApiConfigured() {
  return std::strlen(QWEATHER_API_BASE_URL) > 0 &&
         std::strlen(QWEATHER_API_KEY) > 0;
}

WeatherSyncResult weatherApiConfigMissingResult() {
  return failedResult("Weather API config missing");
}

String weatherApiUrl(const char* path, const String& query) {
  return String(QWEATHER_API_BASE_URL) + path + "?key=" +
         Request::urlEncode(QWEATHER_API_KEY) + query;
}

String airQualityApiUrl(const String& latitude, const String& longitude) {
  return String(QWEATHER_API_BASE_URL) + "/airquality/v1/current/" +
         Request::urlEncode(latitude) + "/" + Request::urlEncode(longitude) +
         "?key=" + Request::urlEncode(QWEATHER_API_KEY);
}

bool parseCompressedJson(const String& json, JsonDocument& doc,
                         const char* responseName) {
  uint8_t* compressed = (uint8_t*)json.c_str();
  uint32_t compressedSize = json.length();
  uint8_t* decompressed = nullptr;
  uint32_t decompressedSize = 0;
  ArduinoUZlib::decompress(compressed, compressedSize, decompressed,
                           decompressedSize);
  if (decompressed == nullptr) {
    Serial.printf("[WeatherService] Failed to decompress %s response\n",
                  responseName);
    return false;
  }

  DeserializationError error = deserializeJson(doc, decompressed);
  free(decompressed);

  if (error) {
    Serial.printf("[WeatherService] Failed to parse %s response\n",
                  responseName);
    return false;
  }

  return true;
}

}  // namespace

WeatherService& WeatherService::getInstance() {
  static WeatherService instance;
  return instance;
}

WeatherSyncResult WeatherService::updateCityLookup(String city, String adm) {
  _cityLookupSyncResult = syncingResult();
  if (!isWeatherApiConfigured()) {
    _cityLookupSyncResult = weatherApiConfigMissingResult();
    Serial.println("[WeatherService] Weather API config missing");
    return _cityLookupSyncResult;
  }

  String url = weatherApiUrl("/geo/v2/city/lookup",
                             "&location=" + Request::urlEncode(city));
  if (adm.length() > 0) {
    url += "&adm=" + Request::urlEncode(adm);
  }
  Request::Response response = Request::getInstance().get(url);
  if (!response.success) {
    _cityLookupSyncResult = failedResult(response.error);
    Serial.printf("[WeatherService] Failed to update cityLookup: %s\n",
                  response.error.c_str());
    return _cityLookupSyncResult;
  }
  if (!parseCityLookupResp(response.body, _cityLookup)) {
    _cityLookupSyncResult = failedResult("Failed to parse cityLookup response");
    Serial.println("[WeatherService] Failed to parse cityLookup response");
    return _cityLookupSyncResult;
  }
  _cityLookupSyncResult = readyResult();
  return _cityLookupSyncResult;
}

bool WeatherService::parseCityLookupResp(String json, CityLookup& cityLookup) {
  JsonDocument doc;
  if (!parseCompressedJson(json, doc, "cityLookup")) {
    return false;
  }

  if (isApiCodeOk(doc) && doc["location"].size() > 0) {
    JsonObject location = doc["location"][0];
    cityLookup.id = location["id"].as<String>();
    cityLookup.name = location["name"].as<String>();
    cityLookup.lat = location["lat"].as<String>();
    cityLookup.lon = location["lon"].as<String>();
    return true;
  }
  Serial.printf("[WeatherService] City lookup has no data: %s\n",
                apiCodeMessage(doc).c_str());
  return false;
}

WeatherSyncResult WeatherService::updateWeatherNow(String location) {
  _weatherNowSyncResult = syncingResult();
  if (!isWeatherApiConfigured()) {
    _weatherNowSyncResult = weatherApiConfigMissingResult();
    Serial.println("[WeatherService] Weather API config missing");
    return _weatherNowSyncResult;
  }

  String url = weatherApiUrl("/v7/weather/now",
                             "&location=" + Request::urlEncode(location));
  Request::Response response = Request::getInstance().get(url);
  if (!response.success) {
    _weatherNowSyncResult = failedResult(response.error);
    Serial.printf("[WeatherService] Failed to update weatherNow: %s\n",
                  response.error.c_str());
    return _weatherNowSyncResult;
  }
  if (!parseWeatherNowResp(response.body, _weatherNow)) {
    _weatherNowSyncResult = failedResult("Failed to parse weatherNow response");
    Serial.println("[WeatherService] Failed to parse weatherNow response");
    return _weatherNowSyncResult;
  }
  _weatherNowSyncResult = readyResult();
  return _weatherNowSyncResult;
}

bool WeatherService::parseWeatherNowResp(String json, WeatherNow& weatherNow) {
  JsonDocument doc;
  if (!parseCompressedJson(json, doc, "weatherNow")) {
    return false;
  }

  JsonObject now = doc["now"];
  if (isApiCodeOk(doc) && !now.isNull()) {
    weatherNow.temp = now["temp"].as<String>();
    weatherNow.feelsLike = now["feelsLike"].as<String>();
    weatherNow.icon = now["icon"].as<String>();
    weatherNow.text = now["text"].as<String>();
    weatherNow.windDir = now["windDir"].as<String>();
    weatherNow.windScale = now["windScale"].as<String>();
    weatherNow.humidity = now["humidity"].as<String>();
    weatherNow.vis = now["vis"].as<String>();
    return true;
  }
  Serial.printf("[WeatherService] weatherNow has no data: %s\n",
                apiCodeMessage(doc).c_str());
  return false;
}

WeatherSyncResult WeatherService::updateWeather7Days(String location) {
  _weather7DaysSyncResult = syncingResult();
  if (!isWeatherApiConfigured()) {
    _weather7DaysSyncResult = weatherApiConfigMissingResult();
    Serial.println("[WeatherService] Weather API config missing");
    return _weather7DaysSyncResult;
  }

  String url = weatherApiUrl("/v7/weather/7d",
                             "&location=" + Request::urlEncode(location));
  Request::Response response = Request::getInstance().get(url);
  if (!response.success) {
    _weather7DaysSyncResult = failedResult(response.error);
    Serial.printf("[WeatherService] Failed to update weather7Days: %s\n",
                  response.error.c_str());
    return _weather7DaysSyncResult;
  }
  if (!parseWeather7DaysResp(response.body, _weather7Days)) {
    _weather7DaysSyncResult =
        failedResult("Failed to parse weather7Days response");
    Serial.println("[WeatherService] Failed to parse weather7Days response");
    return _weather7DaysSyncResult;
  }
  _weather7DaysSyncResult = readyResult();
  return _weather7DaysSyncResult;
}

bool WeatherService::parseWeather7DaysResp(String json,
                                           WeatherDaily weather7Days[]) {
  JsonDocument doc;
  if (!parseCompressedJson(json, doc, "weather7Days")) {
    return false;
  }

  if (isApiCodeOk(doc) && doc["daily"].size() > 0) {
    JsonArray daily = doc["daily"];
    size_t dailyCount = daily.size();
    if (dailyCount > kWeatherDailyCount) {
      dailyCount = kWeatherDailyCount;
    }

    for (size_t i = 0; i < dailyCount; i++) {
      JsonObject day = daily[i];
      weather7Days[i].fxDate = day["fxDate"].as<String>();
      weather7Days[i].sunrise = day["sunrise"].as<String>();
      weather7Days[i].sunset = day["sunset"].as<String>();
      weather7Days[i].tempMax = day["tempMax"].as<String>();
      weather7Days[i].tempMin = day["tempMin"].as<String>();
      weather7Days[i].iconDay = day["iconDay"].as<String>();
      weather7Days[i].textDay = day["textDay"].as<String>();
      weather7Days[i].iconNight = day["iconNight"].as<String>();
      weather7Days[i].textNight = day["textNight"].as<String>();
    }
    for (size_t i = dailyCount; i < kWeatherDailyCount; i++) {
      weather7Days[i] = WeatherDaily{};
    }
    return true;
  }
  Serial.printf("[WeatherService] weather7Days has no data: %s\n",
                apiCodeMessage(doc).c_str());
  return false;
}

WeatherSyncResult WeatherService::updateAirQualityNow(const String& latitude,
                                                      const String& longitude) {
  _airQualityNowSyncResult = syncingResult();
  if (!isWeatherApiConfigured()) {
    _airQualityNowSyncResult = weatherApiConfigMissingResult();
    Serial.println("[WeatherService] Weather API config missing");
    return _airQualityNowSyncResult;
  }

  if (latitude.isEmpty() || longitude.isEmpty()) {
    _airQualityNowSyncResult = failedResult("Location coordinate missing");
    Serial.println("[WeatherService] Location coordinate missing");
    return _airQualityNowSyncResult;
  }

  Request::Response response =
      Request::getInstance().get(airQualityApiUrl(latitude, longitude));
  if (!response.success) {
    _airQualityNowSyncResult = failedResult(response.error);
    Serial.printf("[WeatherService] Failed to update airQualityNow: %s\n",
                  response.error.c_str());
    return _airQualityNowSyncResult;
  }
  if (!parseAirQualityNowResp(response.body, _airQualityNow)) {
    _airQualityNowSyncResult =
        failedResult("Failed to parse airQualityNow response");
    Serial.println("[WeatherService] Failed to parse airQualityNow response");
    return _airQualityNowSyncResult;
  }
  _airQualityNowSyncResult = readyResult();
  return _airQualityNowSyncResult;
}

bool WeatherService::parseAirQualityNowResp(String json,
                                            AirQualityNow& airQualityNow) {
  JsonDocument doc;
  if (!parseCompressedJson(json, doc, "airQualityNow")) {
    return false;
  }

  JsonObject index = doc["indexes"][0];
  if (index.isNull()) {
    Serial.println("[WeatherService] airQualityNow indexes empty");
    return false;
  }

  airQualityNow.aqi = index["aqiDisplay"].as<String>();
  if (airQualityNow.aqi.isEmpty()) {
    airQualityNow.aqi = index["aqi"].as<String>();
  }
  airQualityNow.category = index["category"].as<String>();
  airQualityNow.primaryPollutant = index["primaryPollutant"]["name"].as<String>();
  airQualityNow.pm2p5 = "";

  JsonArray pollutants = doc["pollutants"];
  for (JsonObject pollutant : pollutants) {
    String code = pollutant["code"].as<String>();
    String name = pollutant["name"].as<String>();
    if (code == "pm2p5" || name == "PM2.5" || name == "细颗粒物") {
      airQualityNow.pm2p5 = pollutant["concentration"]["value"].as<String>();
      break;
    }
  }

  return !airQualityNow.aqi.isEmpty() || !airQualityNow.category.isEmpty();
}
