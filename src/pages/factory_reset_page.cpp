#include "factory_reset_page.h"

#include "app/device_config_provider.h"

void FactoryResetPage::create() {
  pageObject = createTransparentPage();
  resetPanel = createCardPanel(212, 144);
  lv_obj_align(resetPanel, LV_ALIGN_CENTER, 0, 0);

  resetLabel =
      createTextLabel(resetPanel, "恢复出厂设置\n长按：确认恢复\n双击：返回天气页");
  lv_obj_set_style_text_align(resetLabel, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_width(resetLabel, LV_PCT(92));
  lv_obj_align(resetLabel, LV_ALIGN_CENTER, 0, 0);

  hide();  // 默认隐藏
}

void FactoryResetPage::update() {
  const char* text =
      resetDone ? "已恢复出厂设置\n请重启设备\n双击：返回天气页"
                : "恢复出厂设置\n长按：确认恢复\n双击：返回天气页";
  lv_label_set_text(resetLabel, text);
  lv_obj_align(resetLabel, LV_ALIGN_CENTER, 0, 0);
}

void FactoryResetPage::onButtonLongPress() {
  DeviceConfigProvider().factoryReset();
  resetDone = true;
  update();
}
