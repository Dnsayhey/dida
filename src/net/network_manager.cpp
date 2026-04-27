#include "network_manager.h"

#include "esp_wifi.h"
#include "esp_wifi_types.h"

namespace {

bool gWifiEventRegistered = false;
uint8_t gLastDisconnectReason = 0;

void registerWifiDiagnostics() {
  if (gWifiEventRegistered) {
    return;
  }

  WiFi.onEvent(
      [](arduino_event_id_t event, arduino_event_info_t info) {
        if (event != ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
          return;
        }

        gLastDisconnectReason = info.wifi_sta_disconnected.reason;
      },
      ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  gWifiEventRegistered = true;
}

}  // namespace

NetworkManager& NetworkManager::getInstance() {
  static NetworkManager instance;
  return instance;
}

bool NetworkManager::connect(const char* ssid, const char* password,
                             uint32_t timeoutMs) {
  registerWifiDiagnostics();
  gLastDisconnectReason = 0;

  WiFi.persistent(false);
  WiFi.disconnect(true, false);
  delay(200);
  configureStationMode();

  WiFi.begin(ssid, password);
  Serial.printf("[NetworkManager] Connecting to SSID: %s\n", ssid);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs) {
    delay(200);
  }
  bool ok = WiFi.status() == WL_CONNECTED;
  if (ok) {
    Serial.printf("[NetworkManager] Connected. IP: %s\n",
                  WiFi.localIP().toString().c_str());
  } else {
    logConnectionFailure(WiFi.status());
  }
  return ok;
}

bool NetworkManager::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}

void NetworkManager::configureStationMode() const {
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);

  wifi_country_t country = {"CN", 1, 13, 20, WIFI_COUNTRY_POLICY_MANUAL};
  if (esp_wifi_set_country(&country) != ESP_OK) {
    Serial.println("[NetworkManager] Failed to set WiFi country CN");
  }
}

void NetworkManager::logConnectionFailure(wl_status_t status) const {
  Serial.printf("[NetworkManager] Connect timeout or failed. status: %d\n",
                static_cast<int>(status));
  if (gLastDisconnectReason == 0) {
    Serial.println("[NetworkManager] Last disconnect reason: none");
    return;
  }

  Serial.printf("[NetworkManager] Last disconnect reason: %u (%s)\n",
                static_cast<unsigned int>(gLastDisconnectReason),
                WiFi.disconnectReasonName(
                    static_cast<wifi_err_reason_t>(gLastDisconnectReason)));
}
