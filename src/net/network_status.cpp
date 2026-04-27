#include "network_status.h"

#include <WiFi.h>

bool NetworkStatus::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}
