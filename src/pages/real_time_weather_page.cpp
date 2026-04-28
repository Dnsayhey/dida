#include "real_time_weather_page.h"

#include <Arduino.h>  // 包含Arduino头文件，其中包含时间相关定义

#include "WString.h"
#include "sources/font/provice_city_district_32.c"

namespace {

constexpr uint32_t kClockRefreshMs = 200;
constexpr uint32_t kWeatherRefreshMs = 60UL * 1000;

}

void RealTimeWeatherPage::create() {
  LV_FONT_DECLARE(provice_city_district_32);

  pageObject = createTransparentPage();

  districtLabel = createPageLabel(&provice_city_district_32);
  CurrentWeatherViewData weatherViewData =
      _weatherViewDataProvider.getCurrentWeatherViewData();
  lv_label_set_text(districtLabel, weatherViewData.locationName.c_str());
  lv_obj_align(districtLabel, LV_ALIGN_TOP_MID, 0, 16);

  dateLabel = createPageLabel();
  lv_obj_align(dateLabel, LV_ALIGN_TOP_MID, 0, 50);

  timeLabel = createPageLabel(&lv_font_montserrat_24);
  lv_obj_set_style_text_letter_space(timeLabel, 2, 0);
  lv_obj_align(timeLabel, LV_ALIGN_TOP_MID, 0, 94);

  weatherLabel = createPageLabel();
  lv_obj_align(weatherLabel, LV_ALIGN_TOP_MID, 0, 148);

  detailTopLabel = createSingleLineLabel(210);
  lv_obj_align(detailTopLabel, LV_ALIGN_TOP_MID, 0, 184);

  detailBottomLabel = createSingleLineLabel(210);
  lv_obj_align(detailBottomLabel, LV_ALIGN_TOP_MID, 0, 210);

  airQualityLabel = createSingleLineLabel(188);
  lv_obj_align(airQualityLabel, LV_ALIGN_BOTTOM_MID, 0, -50);

  pm2p5Label = createSingleLineLabel(188);
  lv_obj_align(pm2p5Label, LV_ALIGN_BOTTOM_MID, 0, -26);

  clockUpdateTimer = lv_timer_create(
      [](lv_timer_t* timer) {
        RealTimeWeatherPage* self =
            static_cast<RealTimeWeatherPage*>(lv_timer_get_user_data(timer));
        self->updateClock();
      },
      kClockRefreshMs, this);

  weatherUpdateTimer = lv_timer_create(
      [](lv_timer_t* timer) {
        RealTimeWeatherPage* self =
            static_cast<RealTimeWeatherPage*>(lv_timer_get_user_data(timer));
        self->updateWeather();
      },
      kWeatherRefreshMs, this);

  update();

  hide();  // 默认隐藏
}

lv_obj_t* RealTimeWeatherPage::createPageLabel(const lv_font_t* font) {
  lv_obj_t* label = lv_label_create(pageObject);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_width(label, LV_PCT(90));
  if (font) {
    lv_obj_set_style_text_font(label, font, 0);
  } else {
    applyUiTextFont(label);
  }
  lv_label_set_text(label, "");
  return label;
}

lv_obj_t* RealTimeWeatherPage::createSingleLineLabel(lv_coord_t width) {
  lv_obj_t* label = createPageLabel();
  lv_obj_set_size(label, width, 23);
  lv_label_set_long_mode(label, LV_LABEL_LONG_MODE_DOTS);
  return label;
}

void RealTimeWeatherPage::update() {
  updateClock();
  updateWeather();
}

void RealTimeWeatherPage::updateClock() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    // 格式化日期字符串 (例如: 2023年12月25日 星期一)
    char dateStr[50];
    const char* weekdayStr[] = {"日", "一", "二", "三", "四", "五", "六"};
    strftime(dateStr, sizeof(dateStr), "%m月%d日 周", &timeinfo);
    strcat(dateStr, weekdayStr[timeinfo.tm_wday]);

    // 格式化时间字符串 (例如: 14:30:25)
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

    // 更新标签显示
    lv_label_set_text(dateLabel, dateStr);
    lv_label_set_text(timeLabel, timeStr);
  }
}

void RealTimeWeatherPage::updateWeather() {
  CurrentWeatherViewData weatherViewData =
      _weatherViewDataProvider.getCurrentWeatherViewData();
  String primaryText = weatherViewData.hasData
                           ? weatherViewData.weatherText + "  " +
                                 weatherViewData.temperatureText
                           : weatherViewData.conditionsText;
  lv_label_set_text(weatherLabel, primaryText.c_str());

  String detailTopText =
      weatherViewData.feelsLikeText + "  " + weatherViewData.humidityText;
  String detailBottomText =
      weatherViewData.windText + "  " + weatherViewData.visibilityText;
  lv_label_set_text(detailTopLabel, detailTopText.c_str());
  lv_label_set_text(detailBottomLabel, detailBottomText.c_str());

  lv_label_set_text(airQualityLabel, weatherViewData.airQualityText.c_str());
  lv_label_set_text(pm2p5Label, weatherViewData.pm2p5Text.c_str());
}
