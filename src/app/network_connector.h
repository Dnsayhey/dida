#ifndef NETWORK_CONNECTOR_H
#define NETWORK_CONNECTOR_H

#include "device_config_provider.h"

class NetworkConnector {
 public:
  bool connectToWifi(const DeviceConfigSnapshot& configSnapshot,
                     uint32_t timeoutMs);
};

#endif  // NETWORK_CONNECTOR_H
