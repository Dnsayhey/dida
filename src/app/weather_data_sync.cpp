#include "weather_data_sync.h"

String WeatherDataSync::ensureLocationId(
    const DeviceConfigSnapshot& configSnapshot,
    DeviceConfigProvider& configProvider) {
  if (!configProvider.isLocationIdConfigured()) {
    configProvider.setLocationId(_weatherDataClient.lookupLocationId(
        configSnapshot.location, configSnapshot.adm));
  }

  return configProvider.getLocationId();
}

WeatherSyncResult WeatherDataSync::syncCurrentConditions(
    const String& locationId) {
  return _weatherDataClient.updateCurrentConditions(locationId);
}

WeatherSyncResult WeatherDataSync::syncDailyForecast(const String& locationId) {
  return _weatherDataClient.updateDailyForecast(locationId);
}
