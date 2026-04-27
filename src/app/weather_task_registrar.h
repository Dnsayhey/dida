#ifndef WEATHER_TASK_REGISTRAR_H
#define WEATHER_TASK_REGISTRAR_H

#include <Arduino.h>

class WeatherDataSync;

class WeatherTaskRegistrar {
 public:
  void registerTasks(const String& locationId, WeatherDataSync& weatherDataSync);
};

#endif  // WEATHER_TASK_REGISTRAR_H
