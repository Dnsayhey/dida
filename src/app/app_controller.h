#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "app_events.h"
#include "page_type.h"
#include "app_state.h"

class AppController {
 public:
  static AppController& getInstance();

  void begin();
  AppCommand handleButtonEvent(ButtonEvent event, PageType currentPage);
  void handleWifiConfigMissing();
  void handleWifiConnecting();
  void handleDataSyncing();
  void handleWeatherReady();

  AppState getState() const { return _state; }

 private:
  AppController() = default;
  AppController(const AppController&) = delete;
  AppController& operator=(const AppController&) = delete;

  void transitionTo(AppState nextState);
  PageType getClickTargetPage(PageType currentPage) const;
  PageType getDoubleClickTargetPage(PageType currentPage) const;

  AppState _state = AppState::Booting;
};

#endif  // APP_CONTROLLER_H
