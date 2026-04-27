#ifndef DEVICE_CONFIG_PROVIDER_H
#define DEVICE_CONFIG_PROVIDER_H

#include <Arduino.h>

struct DeviceConfigSnapshot {
  String wifiSSID;
  String wifiPassword;
  String adm;
  String location;
};

class DeviceConfigProvider {
 public:
  bool isWiFiConfigured() const;
  DeviceConfigSnapshot getStartupConfig() const;
  String getLocationName() const;
  bool isLocationIdConfigured() const;
  String getLocationId() const;
  void setLocationId(const String& locationId);
  bool isLocationCoordinateConfigured() const;
  String getLocationLatitude() const;
  String getLocationLongitude() const;
  void setLocationCoordinate(const String& latitude, const String& longitude);
  void saveProvisionedConfig(const DeviceConfigSnapshot& configSnapshot);
  void applyDevDefaults();
  void factoryReset();
};

#endif  // DEVICE_CONFIG_PROVIDER_H
