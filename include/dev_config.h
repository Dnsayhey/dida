#ifndef DEV_CONFIG_H
#define DEV_CONFIG_H

#if __has_include("dev_config_local.h")
#include "dev_config_local.h"
#endif

#ifndef DEV_WIFI_SSID
#define DEV_WIFI_SSID ""
#endif

#ifndef DEV_WIFI_PASSWORD
#define DEV_WIFI_PASSWORD ""
#endif

#ifndef DEV_ADM
#define DEV_ADM ""
#endif

#ifndef DEV_LOCATION
#define DEV_LOCATION ""
#endif

#endif  // DEV_CONFIG_H
