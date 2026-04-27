#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Preferences.h>

#define NVS_NAMESPACE "dida"
#define WIFI_SSID_KEY "ssid"
#define WIFI_PASSWORD_KEY "pwd"
#define ADM_KEY "adm"
#define LOCATION_KEY "loc"
#define LOCATION_ID_KEY "loc_id"
#define LOCATION_LAT_KEY "loc_lat"
#define LOCATION_LON_KEY "loc_lon"
#define BACKLIGHT_MODE_KEY "blm"
#define THEME_MODE_KEY "theme"

class ConfigManager {
 public:
  static ConfigManager& getInstance();

  void setWiFiSSID(const String& ssid);
  bool isWiFiSSIDSet() const;
  String getWiFiSSID() const;

  void setWiFiPassword(const String& password);
  bool isWiFiPasswordSet() const;
  String getWiFiPassword() const;

  void setAdm(const String& adm);
  bool isAdmSet() const;
  String getAdm() const;

  void setLocation(const String& location);
  bool isLocationSet() const;
  String getLocation() const;

  void setLocationID(const String& location_id);
  bool isLocationIDSet() const;
  String getLocationID() const;

  void setLocationLat(const String& latitude);
  bool isLocationLatSet() const;
  String getLocationLat() const;

  void setLocationLon(const String& longitude);
  bool isLocationLonSet() const;
  String getLocationLon() const;

  void setBacklightMode(int mode);
  bool isBacklightModeSet() const;
  int getBacklightMode() const;

  void setThemeMode(int mode);
  bool isThemeModeSet() const;
  int getThemeMode() const;

  bool isWiFiConfigured() const;

  void factoryReset();

 private:
  ConfigManager() = default;
  ConfigManager(const ConfigManager&) = delete;
  ConfigManager& operator=(const ConfigManager&) = delete;

  mutable Preferences _pref;

  void setString(const String& key, const String& value);
  String getString(const String& key, const String& default_value = "") const;

  void setInt(const String& key, int value);
  int getInt(const String& key, int default_value = 0) const;

  bool hasKey(const String& key) const;
};

#endif
