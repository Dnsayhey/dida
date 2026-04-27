#include "device_config_provider.h"

#include "config/config_manager.h"
#include "dev_config.h"

namespace {

void setStringIfMissing(ConfigManager& config, bool exists,
                        void (ConfigManager::*setter)(const String&),
                        const String& value, const char* keyName) {
  if (value.isEmpty()) {
    Serial.printf("[DeviceConfigProvider] skip empty DEV_MODE default for %s\n",
                  keyName);
    return;
  }

  if (exists) {
    Serial.printf("[DeviceConfigProvider] keep existing %s in DEV_MODE\n",
                  keyName);
    return;
  }

  Serial.printf("[DeviceConfigProvider] apply DEV_MODE default for %s\n",
                keyName);
  (config.*setter)(value);
}

}  // namespace

bool DeviceConfigProvider::isWiFiConfigured() const {
  return ConfigManager::getInstance().isWiFiConfigured();
}

DeviceConfigSnapshot DeviceConfigProvider::getStartupConfig() const {
  ConfigManager& config = ConfigManager::getInstance();
  return {config.getWiFiSSID(), config.getWiFiPassword(), config.getAdm(),
          config.getLocation()};
}

String DeviceConfigProvider::getLocationName() const {
  return ConfigManager::getInstance().getLocation();
}

bool DeviceConfigProvider::isLocationIdConfigured() const {
  return !ConfigManager::getInstance().getLocationID().isEmpty();
}

String DeviceConfigProvider::getLocationId() const {
  return ConfigManager::getInstance().getLocationID();
}

void DeviceConfigProvider::setLocationId(const String& locationId) {
  ConfigManager::getInstance().setLocationID(locationId);
}

void DeviceConfigProvider::saveProvisionedConfig(
    const DeviceConfigSnapshot& configSnapshot) {
  ConfigManager& config = ConfigManager::getInstance();
  config.setWiFiSSID(configSnapshot.wifiSSID);
  config.setWiFiPassword(configSnapshot.wifiPassword);
  config.setAdm(configSnapshot.adm);
  config.setLocation(configSnapshot.location);
  config.setLocationID("");
}

void DeviceConfigProvider::applyDevDefaults() {
  Serial.println("[DeviceConfigProvider] applyDevDefaults");
  ConfigManager& config = ConfigManager::getInstance();

  setStringIfMissing(config, config.isWiFiSSIDSet(),
                     &ConfigManager::setWiFiSSID, DEV_WIFI_SSID, "WiFi SSID");
  setStringIfMissing(config, config.isWiFiPasswordSet(),
                     &ConfigManager::setWiFiPassword, DEV_WIFI_PASSWORD,
                     "WiFi password");
  setStringIfMissing(config, config.isAdmSet(), &ConfigManager::setAdm, DEV_ADM,
                     "adm");
  setStringIfMissing(config, config.isLocationSet(), &ConfigManager::setLocation,
                     DEV_LOCATION, "location");
}

void DeviceConfigProvider::factoryReset() {
  ConfigManager::getInstance().factoryReset();
}
