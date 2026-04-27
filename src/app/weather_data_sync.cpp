#include "weather_data_sync.h"

void WeatherDataSync::ensureLocation(
    const DeviceConfigSnapshot& configSnapshot,
    DeviceConfigProvider& configProvider) {
  if (configProvider.isLocationIdConfigured() &&
      configProvider.isLocationCoordinateConfigured()) {
    return;
  }

  CityLookup cityLookup =
      _weatherDataClient.lookupLocation(configSnapshot.location,
                                        configSnapshot.adm);
  if (!cityLookup.id.isEmpty()) {
    configProvider.setLocationId(cityLookup.id);
  }
  if (!cityLookup.lat.isEmpty() && !cityLookup.lon.isEmpty()) {
    configProvider.setLocationCoordinate(cityLookup.lat, cityLookup.lon);
  }
}

WeatherSyncResult WeatherDataSync::syncCurrentConditions(
    const String& locationId) {
  return _weatherDataClient.updateCurrentConditions(locationId);
}

WeatherSyncResult WeatherDataSync::syncDailyForecast(const String& locationId) {
  return _weatherDataClient.updateDailyForecast(locationId);
}

WeatherSyncResult WeatherDataSync::syncAirQualityNow(const String& latitude,
                                                     const String& longitude) {
  return _weatherDataClient.updateAirQualityNow(latitude, longitude);
}
