#ifndef WEATHER_TYPE_H
#define WEATHER_TYPE_H

#include <Arduino.h>

typedef struct CityLookup {
  String name;  // 城市名称
  String id;    // location id
  String lat;   // 经度
  String lon;   // 纬度
} CityLookup;

typedef struct WeatherNow {
  String temp;       // 温度
  String feelsLike;  // 体感温度
  String icon;       // 天气图标
  String text;       // 天气描述
  String windDir;    // 风向
  String windScale;  // 风力等级
  String humidity;   // 湿度
  String vis;        // 能见度
} WeatherNow;

typedef struct WeatherDaily {
  String fxDate;     // 日期
  String sunrise;    // 日出时间
  String sunset;     // 日落时间
  String tempMax;    // 最高温度
  String tempMin;    // 最低温度
  String iconDay;    // 白天天气图标
  String textDay;    // 白天天气描述
  String iconNight;  // 夜间天气图标
  String textNight;  // 夜间天气描述
} WeatherDaily;

enum class WeatherSyncStatus {
  Idle,
  Syncing,
  Ready,
  Failed,
};

struct WeatherSyncResult {
  WeatherSyncResult() = default;
  WeatherSyncResult(WeatherSyncStatus status, const String& message,
                    unsigned long updatedAt)
      : status(status), message(message), updatedAt(updatedAt) {}

  WeatherSyncStatus status = WeatherSyncStatus::Idle;
  String message;
  unsigned long updatedAt = 0;
};

#endif
