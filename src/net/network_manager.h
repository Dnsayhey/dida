#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <WiFi.h>

class NetworkManager {
 public:
  static NetworkManager& getInstance();

  bool connect(const char* ssid, const char* password,
               uint32_t timeoutMs = 10000);
  bool isConnected() const;

 private:
  NetworkManager() = default;
  NetworkManager(const NetworkManager&) = delete;
  NetworkManager& operator=(const NetworkManager&) = delete;

  void configureStationMode() const;
  void logConnectionFailure(wl_status_t status) const;
};

#endif
