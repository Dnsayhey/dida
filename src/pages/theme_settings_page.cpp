#include "theme_settings_page.h"

#include <Arduino.h>

#include "app/device_settings_controller.h"
#include "page_manager.h"

void ThemeSettingsPage::create() {
  pageObject = createTransparentPage();
  settingsPanel = createCardPanel(212, 168);
  lv_obj_align(settingsPanel, LV_ALIGN_CENTER, 0, 0);
  settingsLabel = createTextLabel(settingsPanel);
  lv_obj_set_style_text_align(settingsLabel, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_width(settingsLabel, LV_PCT(92));
  lv_obj_align(settingsLabel, LV_ALIGN_CENTER, 0, 0);

  update();

  hide();  // 默认隐藏
}

void ThemeSettingsPage::update() {
  DeviceSettingsController settings;
  String text = "亮度与主题\n";
  text += "亮度：";
  text += settings.getBrightnessModeText();

  text += "\n主题：";
  text += settings.isDarkTheme() ? "深色" : "浅色";
  text += "\n单击：切换亮度";
  text += "\n长按：切换主题";

  // 设置新文本并居中显示
  lv_label_set_text(settingsLabel, text.c_str());
  lv_obj_align(settingsLabel, LV_ALIGN_CENTER, 0, 0);
}

void ThemeSettingsPage::onButtonClick() {
  DeviceSettingsController().switchToNextBrightnessMode();
  update();
}

void ThemeSettingsPage::onButtonLongPress() {
  DeviceSettingsController().toggleTheme();
  PageManager::getInstance().applyTheme();
  update();
}
