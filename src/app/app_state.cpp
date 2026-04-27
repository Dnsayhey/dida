#include "app_state.h"

const char* appStateName(AppState state) {
  switch (state) {
    case AppState::Booting:
      return "Booting";
    case AppState::WaitingForWifiConfig:
      return "WaitingForWifiConfig";
    case AppState::ConnectingWifi:
      return "ConnectingWifi";
    case AppState::SyncingData:
      return "SyncingData";
    case AppState::ShowingWeather:
      return "ShowingWeather";
    case AppState::Error:
      return "Error";
    default:
      return "Unknown";
  }
}
