#include "network_connector.h"

#include "net/network_manager.h"

bool NetworkConnector::connectToWifi(
    const DeviceConfigSnapshot& configSnapshot, uint32_t timeoutMs) {
  return NetworkManager::getInstance().connect(configSnapshot.wifiSSID.c_str(),
                                               configSnapshot.wifiPassword.c_str(),
                                               timeoutMs);
}
