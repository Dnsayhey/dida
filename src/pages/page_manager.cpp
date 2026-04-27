#include "page_manager.h"

#include <Arduino.h>

#include "factory_reset_page.h"
#include "future_weather_page.h"
#include "init_page.h"
#include "network_setup_page.h"
#include "real_time_weather_page.h"
#include "theme_settings_page.h"
#include "ui_theme.h"

PageManager& PageManager::getInstance() {
  static PageManager instance;
  return instance;
}

PageManager::PageManager() : initialized(false), currentPage(PAGE_INIT) {
  // 初始化页面指针数组
  static InitPage initPage;
  static NetworkSetupPage networkSetupPage;
  static RealTimeWeatherPage realTimeWeatherPage;
  static FutureWeatherPage futureWeatherPage;
  static ThemeSettingsPage themeSettingsPage;
  static FactoryResetPage factoryResetPage;

  pages[PAGE_INIT] = &initPage;
  pages[PAGE_NETWORK_SETUP] = &networkSetupPage;
  pages[PAGE_REAL_TIME_WEATHER] = &realTimeWeatherPage;
  pages[PAGE_FUTURE_WEATHER] = &futureWeatherPage;
  pages[PAGE_THEME_SETTINGS] = &themeSettingsPage;
  pages[PAGE_FACTORY_RESET] = &factoryResetPage;
}

void PageManager::init() {
  if (initialized) {
    return;
  }

  // 等待屏幕完全初始化
  delay(200);

  // 清屏，确保显示干净
  lv_obj_clean(lv_scr_act());

  // 创建所有页面
  for (int i = 0; i < 6; i++) {
    pages[i]->create();
  }
  applyTheme();

  // 显示初始页面
  initialized = true;
  pages[currentPage]->show();
  pages[currentPage]->update();
  Serial.printf("[PageManager] init -> %s\n", getPageName(currentPage));
}

void PageManager::update() {
  if (!initialized) {
    return;
  }

  // 更新当前页面
  pages[currentPage]->update();
}

void PageManager::applyTheme() {
  lv_obj_set_style_bg_color(lv_scr_act(), UiTheme::current().background, 0);
  lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);

  for (int i = 0; i < 6; i++) {
    if (pages[i]) {
      pages[i]->applyTheme();
    }
  }
}

void PageManager::switchToPage(PageType page) {
  if (page == currentPage) return;

  Serial.printf("[PageManager] switch %s -> %s\n", getPageName(currentPage),
                getPageName(page));
  if (!initialized) {
    currentPage = page;
    return;
  }

  pages[currentPage]->hide();
  currentPage = page;
  applyTheme();
  pages[currentPage]->show();
  pages[currentPage]->update();
}

void PageManager::dispatchClickToCurrentPage() {
  if (!initialized) {
    return;
  }

  Serial.printf("[PageManager] click dispatch on %s\n",
                getPageName(currentPage));
  if (pages[currentPage]) {
    pages[currentPage]->onButtonClick();
  }
}

void PageManager::dispatchLongPressToCurrentPage() {
  if (!initialized) {
    return;
  }

  Serial.printf("[PageManager] long press dispatch on %s\n",
                getPageName(currentPage));
  if (pages[currentPage]) {
    pages[currentPage]->onButtonLongPress();
  }
}

const char* PageManager::getPageName(PageType page) const {
  switch (page) {
    case PAGE_INIT:
      return "PAGE_INIT";
    case PAGE_NETWORK_SETUP:
      return "PAGE_NETWORK_SETUP";
    case PAGE_REAL_TIME_WEATHER:
      return "PAGE_REAL_TIME_WEATHER";
    case PAGE_FUTURE_WEATHER:
      return "PAGE_FUTURE_WEATHER";
    case PAGE_THEME_SETTINGS:
      return "PAGE_THEME_SETTINGS";
    case PAGE_FACTORY_RESET:
      return "PAGE_FACTORY_RESET";
    default:
      return "UNKNOWN_PAGE";
  }
}
