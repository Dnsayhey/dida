#include "real_time_weather_page.h"

#include <Arduino.h>  // 包含Arduino头文件，其中包含时间相关定义

#include "WString.h"
#include "sources/font/provice_city_district_32.c"

namespace {

constexpr uint32_t kClockRefreshMs = 200;
constexpr uint32_t kWeatherRefreshMs = 60UL * 1000;

constexpr lv_coord_t kPanelWidth = 212;
constexpr lv_coord_t kHeaderPanelHeight = 78;
constexpr lv_coord_t kWeatherPanelHeight = 112;
constexpr lv_coord_t kAirPanelHeight = 58;

}

void RealTimeWeatherPage::create() {
  LV_FONT_DECLARE(provice_city_district_32);

  pageObject = createTransparentPage();

  headerPanel = createCardPanel(kPanelWidth, kHeaderPanelHeight);
  lv_obj_align(headerPanel, LV_ALIGN_TOP_MID, 0, 10);

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

  weatherPanel = createCardPanel(kPanelWidth, kWeatherPanelHeight);
  lv_obj_align(weatherPanel, LV_ALIGN_TOP_MID, 0, 126);

  weatherLabel = createPageLabel();
  lv_obj_align(weatherLabel, LV_ALIGN_TOP_MID, 0, 136);

  temperatureLabel = createPageLabel();
  lv_obj_align(temperatureLabel, LV_ALIGN_TOP_MID, 0, 164);

  detailLabel = createPageLabel();
  lv_obj_set_style_text_align(detailLabel, LV_TEXT_ALIGN_LEFT, 0);
  lv_obj_set_width(detailLabel, 188);
  lv_obj_align(detailLabel, LV_ALIGN_TOP_MID, 0, 198);

  airPanel = createCardPanel(kPanelWidth, kAirPanelHeight);
  lv_obj_align(airPanel, LV_ALIGN_BOTTOM_MID, 0, -16);

  airQualityLabel = createPageLabel();
  lv_obj_set_width(airQualityLabel, 188);
  lv_obj_align(airQualityLabel, LV_ALIGN_BOTTOM_MID, 0, -50);

  pm2p5Label = createPageLabel();
  lv_obj_set_width(pm2p5Label, 188);
  lv_obj_align(pm2p5Label, LV_ALIGN_BOTTOM_MID, 0, -26);

  weatherStatusLabel = createPageLabel();
  lv_obj_align(weatherStatusLabel, LV_ALIGN_TOP_MID, 0, 224);

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
  lv_label_set_text(weatherLabel,
                    weatherViewData.hasData ? weatherViewData.weatherText.c_str()
                                            : weatherViewData.conditionsText.c_str());
  lv_label_set_text(temperatureLabel, weatherViewData.temperatureText.c_str());

  String detailText = weatherViewData.feelsLikeText + "  " +
                      weatherViewData.humidityText + "\n" +
                      weatherViewData.windText + "  " +
                      weatherViewData.visibilityText;
  lv_label_set_text(detailLabel, detailText.c_str());

  lv_label_set_text(airQualityLabel, weatherViewData.airQualityText.c_str());
  lv_label_set_text(pm2p5Label, weatherViewData.pm2p5Text.c_str());

  if (weatherViewData.statusText.isEmpty()) {
    lv_label_set_text(weatherStatusLabel, "");
    lv_obj_add_flag(weatherStatusLabel, LV_OBJ_FLAG_HIDDEN);
    return;
  }

  lv_label_set_text(weatherStatusLabel, weatherViewData.statusText.c_str());
  lv_obj_clear_flag(weatherStatusLabel, LV_OBJ_FLAG_HIDDEN);
}
