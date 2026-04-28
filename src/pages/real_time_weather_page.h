#ifndef REAL_TIME_WEATHER_PAGE_H
#define REAL_TIME_WEATHER_PAGE_H

#include "app/weather_view_data_provider.h"
#include "base_page.h"

class RealTimeWeatherPage : public BasePage {
 public:
  void create() override;
  void update() override;

 private:
  lv_obj_t* createPageLabel(const lv_font_t* font = nullptr);
  lv_obj_t* createSingleLineLabel(lv_coord_t width, lv_coord_t height,
                                  const lv_font_t* font = nullptr);
  void updateClock();
  void updateWeather();

  lv_obj_t* districtLabel;
  lv_obj_t* dateLabel;
  lv_obj_t* timeLabel;
  lv_obj_t* weatherLabel;
  lv_obj_t* detailTopLabel;
  lv_obj_t* detailBottomLabel;
  lv_obj_t* airQualityLabel;
  lv_obj_t* pm2p5Label;

  lv_timer_t* clockUpdateTimer;
  lv_timer_t* weatherUpdateTimer;
  WeatherViewDataProvider _weatherViewDataProvider;
};

#endif  // REAL_TIME_WEATHER_PAGE_H
