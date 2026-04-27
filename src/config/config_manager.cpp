#include "config_manager.h"

namespace {

class PreferencesSession {
 public:
  PreferencesSession(Preferences& pref, bool readOnly) : _pref(pref) {
    // Opening read-write here creates the namespace after a full flash erase.
    // No value is written unless a setter calls put*().
    _opened = _pref.begin(NVS_NAMESPACE, false);
  }

  ~PreferencesSession() {
    if (_opened) {
      _pref.end();
    }
  }

  bool isOpen() const { return _opened; }

 private:
  Preferences& _pref;
  bool _opened = false;
};

struct WiFiConfigSnapshot {
  String ssid;
  String password;
};

WiFiConfigSnapshot getWiFiConfigSnapshot(const ConfigManager& config) {
  return {config.getWiFiSSID(), config.getWiFiPassword()};
}

bool isSensitiveKey(const String& key) {
  return key == WIFI_PASSWORD_KEY;
}

}  // namespace

ConfigManager& ConfigManager::getInstance() {
  static ConfigManager instance;
  return instance;
}

void ConfigManager::setString(const String& key, const String& value) {
  PreferencesSession session(_pref, false);
  if (!session.isOpen()) {
    Serial.println("[ConfigManager] failed to open NVS for setString");
    return;
  }
  _pref.putString(key.c_str(), value.c_str());
}

String ConfigManager::getString(const String& key,
                                const String& default_value) const {
  PreferencesSession session(_pref, true);
  if (!session.isOpen()) {
    Serial.println("[ConfigManager] failed to open NVS for getString");
    return default_value;
  }
  bool exists = _pref.isKey(key.c_str());
  if (!exists) {
    return default_value;
  }

  String value = _pref.getString(key.c_str(), default_value);
  if (isSensitiveKey(key) && value.isEmpty()) {
    Serial.printf("[ConfigManager] stored sensitive key is empty: %s\n",
                  key.c_str());
  }
  return value;
}

void ConfigManager::setInt(const String& key, int value) {
  PreferencesSession session(_pref, false);
  if (!session.isOpen()) {
    Serial.println("[ConfigManager] failed to open NVS for setInt");
    return;
  }
  _pref.putInt(key.c_str(), value);
}

int ConfigManager::getInt(const String& key, int default_value) const {
  PreferencesSession session(_pref, true);
  if (!session.isOpen()) {
    Serial.println("[ConfigManager] failed to open NVS for getInt");
    return default_value;
  }
  bool exists = _pref.isKey(key.c_str());
  int value = exists ? _pref.getInt(key.c_str(), default_value) : default_value;
  return value;
}

bool ConfigManager::hasKey(const String& key) const {
  PreferencesSession session(_pref, true);
  if (!session.isOpen()) {
    Serial.println("[ConfigManager] failed to open NVS for hasKey");
    return false;
  }
  bool exists = _pref.isKey(key.c_str());
  return exists;
}

void ConfigManager::setWiFiSSID(const String& ssid) {
  setString(WIFI_SSID_KEY, ssid);
}

String ConfigManager::getWiFiSSID() const { return getString(WIFI_SSID_KEY); }

bool ConfigManager::isWiFiSSIDSet() const { return hasKey(WIFI_SSID_KEY); }

void ConfigManager::setWiFiPassword(const String& password) {
  setString(WIFI_PASSWORD_KEY, password);
}

String ConfigManager::getWiFiPassword() const {
  return getString(WIFI_PASSWORD_KEY);
}

bool ConfigManager::isWiFiPasswordSet() const {
  return hasKey(WIFI_PASSWORD_KEY);
}

void ConfigManager::setAdm(const String& adm) { setString(ADM_KEY, adm); }

String ConfigManager::getAdm() const { return getString(ADM_KEY); }

bool ConfigManager::isAdmSet() const { return hasKey(ADM_KEY); }

void ConfigManager::setLocation(const String& location) {
  setString(LOCATION_KEY, location);
}

String ConfigManager::getLocation() const { return getString(LOCATION_KEY); }

bool ConfigManager::isLocationSet() const { return hasKey(LOCATION_KEY); }

void ConfigManager::setLocationID(const String& location_id) {
  setString(LOCATION_ID_KEY, location_id);
}

String ConfigManager::getLocationID() const {
  return getString(LOCATION_ID_KEY);
}

bool ConfigManager::isLocationIDSet() const { return hasKey(LOCATION_ID_KEY); }

void ConfigManager::setBacklightMode(int mode) {
  setInt(BACKLIGHT_MODE_KEY, mode);
}

int ConfigManager::getBacklightMode() const {
  return getInt(BACKLIGHT_MODE_KEY, 0);
}

bool ConfigManager::isBacklightModeSet() const {
  return hasKey(BACKLIGHT_MODE_KEY);
}

void ConfigManager::setThemeMode(int mode) { setInt(THEME_MODE_KEY, mode); }

int ConfigManager::getThemeMode() const { return getInt(THEME_MODE_KEY, 0); }

bool ConfigManager::isThemeModeSet() const { return hasKey(THEME_MODE_KEY); }

bool ConfigManager::isWiFiConfigured() const {
  WiFiConfigSnapshot snapshot = getWiFiConfigSnapshot(*this);
  return !snapshot.ssid.isEmpty() && !snapshot.password.isEmpty();
}

void ConfigManager::factoryReset() {
  PreferencesSession session(_pref, false);
  if (!session.isOpen()) {
    Serial.println("[ConfigManager] failed to open NVS for factoryReset");
    return;
  }
  _pref.clear();
}
