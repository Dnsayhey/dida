#include "real_time_weather_page.h"

#include <Arduino.h>  // 包含Arduino头文件，其中包含时间相关定义

#include "WString.h"
#include "layouts/flex/lv_flex.h"
#include "layouts/lv_layout.h"
#include "sources/font/provice_city_district_32.c"

namespace {

constexpr uint32_t kClockRefreshMs = 200;
constexpr uint32_t kWeatherRefreshMs = 60UL * 1000;

constexpr lv_coord_t kContentWidth = 216;
constexpr lv_coord_t kLargeTextHeight = 32;
constexpr lv_coord_t kTimeTextHeight = 27;
constexpr lv_coord_t kTextHeight = 23;

}

void RealTimeWeatherPage::create() {
  LV_FONT_DECLARE(provice_city_district_32);

  pageObject = createTransparentPage();
  lv_obj_set_layout(pageObject, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(pageObject, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(pageObject, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_top(pageObject, 14, 0);
  lv_obj_set_style_pad_left(pageObject, 12, 0);
  lv_obj_set_style_pad_right(pageObject, 12, 0);
  lv_obj_set_style_pad_row(pageObject, 7, 0);

  districtLabel = createSingleLineLabel(kContentWidth, kLargeTextHeight,
                                        &provice_city_district_32);
  CurrentWeatherViewData weatherViewData =
      _weatherViewDataProvider.getCurrentWeatherViewData();
  lv_label_set_text(districtLabel, weatherViewData.locationName.c_str());

  dateLabel = createSingleLineLabel(kContentWidth, kTextHeight);

  timeLabel = createSingleLineLabel(kContentWidth, kTimeTextHeight,
                                    &lv_font_montserrat_24);
  lv_obj_set_style_text_letter_space(timeLabel, 2, 0);
  lv_obj_set_style_margin_top(timeLabel, 16, 0);

  weatherLabel = createSingleLineLabel(kContentWidth, kTextHeight);
  lv_obj_set_style_margin_top(weatherLabel, 12, 0);

  detailTopLabel = createSingleLineLabel(kContentWidth, kTextHeight);
  lv_obj_set_style_margin_top(detailTopLabel, 12, 0);

  detailBottomLabel = createSingleLineLabel(kContentWidth, kTextHeight);

  airQualityLabel = createSingleLineLabel(kContentWidth, kTextHeight);
  lv_obj_set_style_margin_top(airQualityLabel, 12, 0);

  pm2p5Label = createSingleLineLabel(kContentWidth, kTextHeight);

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

lv_obj_t* RealTimeWeatherPage::createSingleLineLabel(lv_coord_t width,
                                                     lv_coord_t height,
                                                     const lv_font_t* font) {
  lv_obj_t* label = createPageLabel(font);
  lv_obj_set_size(label, width, height);
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
