#ifndef STARTUP_FLOW_H
#define STARTUP_FLOW_H

#include <Arduino.h>

#include "app_command_executor.h"
#include "device_config_provider.h"
#include "network_connector.h"
#include "provisioning_portal.h"
#include "weather_data_sync.h"
#include "weather_task_registrar.h"

class DisplayDevice;
class PageManager;

class StartupFlow {
 public:
  void run(DisplayDevice& displayDevice, PageManager& pageManager);

 private:
  void showNetworkSetupWhenNeeded(DisplayDevice& displayDevice,
                                  PageManager& pageManager);
  void waitForWifiConfiguration(DisplayDevice& displayDevice,
                                PageManager& pageManager,
                                bool forceProvisioning = false);
  DeviceConfigSnapshot connectToWifiWithProvisioning(
      DisplayDevice& displayDevice, PageManager& pageManager);
  void switchToWeatherPage(DisplayDevice& displayDevice,
                           PageManager& pageManager);
  void executePageCommand(DisplayDevice& displayDevice,
                          PageManager& pageManager,
                          const AppCommand& command);
  void updateCurrentPage(DisplayDevice& displayDevice,
                         PageManager& pageManager);

  DeviceConfigProvider _configProvider;
  NetworkConnector _networkConnector;
  ProvisioningPortal _provisioningPortal;
  WeatherDataSync _weatherDataSync;
  WeatherTaskRegistrar _weatherTaskRegistrar;
  AppCommandExecutor _commandExecutor;
};

#endif  // STARTUP_FLOW_H
