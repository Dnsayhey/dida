#include "future_weather_page.h"

#include "layouts/flex/lv_flex.h"
#include "layouts/lv_layout.h"
#include "misc/lv_area.h"
#include "widgets/label/lv_label.h"

namespace {

constexpr uint32_t kFutureWeatherRefreshMs = 60UL * 1000;
constexpr int kForecastDays = 6;

}

void FutureWeatherPage::create() {
  pageObject = createTransparentPage();
  lv_obj_set_layout(pageObject, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(pageObject, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_top(pageObject, 12, 0);
  lv_obj_set_style_pad_left(pageObject, 14, 0);
  lv_obj_set_style_pad_right(pageObject, 14, 0);
  lv_obj_set_style_pad_row(pageObject, 6, 0);

  titleLabel = createForecastLabel();
  lv_label_set_text(titleLabel, "未来天气");

  for (int i = 0; i < kForecastDays; i++) {
    forecastLabels[i] = createForecastLabel();
  }

  updateTimer = lv_timer_create(
      [](lv_timer_t* timer) {
        FutureWeatherPage* self =
            static_cast<FutureWeatherPage*>(lv_timer_get_user_data(timer));
        self->update();
      },
      kFutureWeatherRefreshMs, this);

  update();

  hide();  // 默认隐藏
}

lv_obj_t* FutureWeatherPage::createForecastLabel() {
  lv_obj_t* label = createTextLabel(pageObject);
  lv_obj_set_size(label, LV_PCT(100), 36);
  markThemeCard(label);
  lv_obj_set_style_border_width(label, 1, 0);
  lv_obj_set_style_radius(label, 10, 0);
  applyThemeToNode(label);
  lv_obj_set_style_pad_left(label, 8, 0);
  lv_obj_set_style_pad_top(label, 5, 0);
  return label;
}

void FutureWeatherPage::update() {
  updateForecastItems();
}

void FutureWeatherPage::updateForecastItems() {
  for (int i = 0; i < kForecastDays; i++) {
    DailyForecastViewData forecastViewData =
        _weatherViewDataProvider.getDailyForecastViewData(i + 1);
    lv_label_set_text(forecastLabels[i], forecastViewData.forecastText.c_str());
  }
}
