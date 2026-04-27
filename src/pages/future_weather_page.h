#ifndef FUTURE_WEATHER_PAGE_H
#define FUTURE_WEATHER_PAGE_H

#include "app/weather_view_data_provider.h"
#include "base_page.h"

class FutureWeatherPage : public BasePage {
 public:
  void create() override;
  void update() override;

 private:
  lv_obj_t* createForecastLabel();
  void updateForecastItems();

  lv_obj_t* titleLabel;
  lv_obj_t* forecastLabels[6];
  lv_timer_t* updateTimer;
  WeatherViewDataProvider _weatherViewDataProvider;
};

#endif  // FUTURE_WEATHER_PAGE_H
