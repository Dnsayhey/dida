#ifndef APP_STATE_H
#define APP_STATE_H

enum class AppState {
  Booting,
  WaitingForWifiConfig,
  ConnectingWifi,
  SyncingData,
  ShowingWeather,
  Error,
};

const char* appStateName(AppState state);

#endif  // APP_STATE_H
