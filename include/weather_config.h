#ifndef WEATHER_CONFIG_H
#define WEATHER_CONFIG_H

#if __has_include("weather_config_local.h")
#include "weather_config_local.h"
#endif

#ifndef QWEATHER_API_BASE_URL
#define QWEATHER_API_BASE_URL ""
#endif

#ifndef QWEATHER_API_KEY
#define QWEATHER_API_KEY ""
#endif

#endif  // WEATHER_CONFIG_H
