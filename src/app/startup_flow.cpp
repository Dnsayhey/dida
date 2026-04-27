#include "startup_flow.h"

#include "app_controller.h"
#include "board/board_config.h"
#include "hardware/display_device.h"
#include "pages/page_manager.h"

void StartupFlow::run(DisplayDevice& displayDevice, PageManager& pageManager) {
  DeviceConfigSnapshot configSnapshot =
      connectToWifiWithProvisioning(displayDevice, pageManager);

  AppController::getInstance().handleDataSyncing();
  String locationId =
      _weatherDataSync.ensureLocationId(configSnapshot, _configProvider);
  _weatherTaskRegistrar.registerTasks(locationId, _weatherDataSync);
  AppController::getInstance().handleWeatherReady();
  switchToWeatherPage(displayDevice, pageManager);
}

void StartupFlow::showNetworkSetupWhenNeeded(DisplayDevice& displayDevice,
                                             PageManager& pageManager) {
  if (_configProvider.isWiFiConfigured()) {
    return;
  }

  Serial.println("init_and_api_task switch to network setup page");
  AppController::getInstance().handleWifiConfigMissing();
  executePageCommand(
      displayDevice, pageManager,
      AppCommand(AppCommandType::SwitchPage, PAGE_NETWORK_SETUP));
}

void StartupFlow::waitForWifiConfiguration(DisplayDevice& displayDevice,
                                           PageManager& pageManager,
                                           bool forceProvisioning) {
  bool wifiConfigured = _configProvider.isWiFiConfigured();
  if (wifiConfigured && !forceProvisioning) {
    return;
  }

  Serial.println(
      "init_and_api_task WiFi not configured, start provisioning portal");
  if (!_provisioningPortal.begin()) {
    Serial.println("init_and_api_task provisioning portal failed to start");
  }
  updateCurrentPage(displayDevice, pageManager);

  while (!_provisioningPortal.isConfigured()) {
    _provisioningPortal.handleClient();
    vTaskDelay(pdMS_TO_TICKS(20));
  }

  _provisioningPortal.stop();
}

DeviceConfigSnapshot StartupFlow::connectToWifiWithProvisioning(
    DisplayDevice& displayDevice, PageManager& pageManager) {
  while (true) {
    showNetworkSetupWhenNeeded(displayDevice, pageManager);
    waitForWifiConfiguration(displayDevice, pageManager);

    DeviceConfigSnapshot configSnapshot = _configProvider.getStartupConfig();
    AppController::getInstance().handleWifiConnecting();
    updateCurrentPage(displayDevice, pageManager);
    if (_networkConnector.connectToWifi(configSnapshot,
                                        BoardConfig::WIFI_CONNECT_TIMEOUT_MS)) {
      return configSnapshot;
    }

    Serial.println(
        "init_and_api_task WiFi connect failed, restart provisioning portal");
    AppController::getInstance().handleWifiConfigMissing();
    executePageCommand(
        displayDevice, pageManager,
        AppCommand(AppCommandType::SwitchPage, PAGE_NETWORK_SETUP));
    waitForWifiConfiguration(displayDevice, pageManager, true);
  }
}

void StartupFlow::switchToWeatherPage(DisplayDevice& displayDevice,
                                      PageManager& pageManager) {
  Serial.println("init_and_api_task switch to real time weather page");
  executePageCommand(
      displayDevice, pageManager,
      AppCommand(AppCommandType::SwitchPage, PAGE_REAL_TIME_WEATHER));
}

void StartupFlow::executePageCommand(DisplayDevice& displayDevice,
                                     PageManager& pageManager,
                                     const AppCommand& command) {
  if (displayDevice.lock(portMAX_DELAY)) {
    _commandExecutor.execute(command, pageManager);
    displayDevice.unlock();
  }
}

void StartupFlow::updateCurrentPage(DisplayDevice& displayDevice,
                                    PageManager& pageManager) {
  if (displayDevice.lock(portMAX_DELAY)) {
    pageManager.update();
    displayDevice.unlock();
  }
}
