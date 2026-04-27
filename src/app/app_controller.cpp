#include "app_controller.h"

#include <Arduino.h>

AppController& AppController::getInstance() {
  static AppController instance;
  return instance;
}

void AppController::begin() {
  Serial.printf("[AppController] begin in %s\n", appStateName(_state));
}

AppCommand AppController::handleButtonEvent(ButtonEvent event,
                                            PageType currentPage) {
  Serial.printf("[AppController] button event: %s on page %d in %s\n",
                buttonEventName(event), currentPage, appStateName(_state));

  switch (event) {
    case ButtonEvent::Click: {
      PageType targetPage = getClickTargetPage(currentPage);
      if (targetPage == currentPage) {
        return {AppCommandType::DispatchPageClick, currentPage};
      }
      return {AppCommandType::SwitchPage, targetPage};
    }
    case ButtonEvent::DoubleClick:
      return {AppCommandType::SwitchPage, getDoubleClickTargetPage(currentPage)};
    case ButtonEvent::LongPress:
      return {AppCommandType::DispatchPageLongPress, currentPage};
    default:
      return {};
  }
}

PageType AppController::getClickTargetPage(PageType currentPage) const {
  switch (currentPage) {
    case PAGE_REAL_TIME_WEATHER:
      return PAGE_FUTURE_WEATHER;
    case PAGE_FUTURE_WEATHER:
      return PAGE_REAL_TIME_WEATHER;
    default:
      return currentPage;
  }
}

PageType AppController::getDoubleClickTargetPage(PageType currentPage) const {
  switch (currentPage) {
    case PAGE_REAL_TIME_WEATHER:
    case PAGE_FUTURE_WEATHER:
      return PAGE_THEME_SETTINGS;
    case PAGE_THEME_SETTINGS:
      return PAGE_FACTORY_RESET;
    case PAGE_FACTORY_RESET:
      return PAGE_REAL_TIME_WEATHER;
    default:
      return currentPage;
  }
}

void AppController::handleWifiConfigMissing() {
  transitionTo(AppState::WaitingForWifiConfig);
}

void AppController::handleWifiConnecting() {
  transitionTo(AppState::ConnectingWifi);
}

void AppController::handleDataSyncing() {
  transitionTo(AppState::SyncingData);
}

void AppController::handleWeatherReady() {
  transitionTo(AppState::ShowingWeather);
}

void AppController::transitionTo(AppState nextState) {
  if (_state == nextState) {
    return;
  }

  Serial.printf("[AppController] state %s -> %s\n", appStateName(_state),
                appStateName(nextState));
  _state = nextState;
}
