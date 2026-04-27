#ifndef THEME_SETTINGS_PAGE_H
#define THEME_SETTINGS_PAGE_H

#include "base_page.h"

class ThemeSettingsPage : public BasePage {
 public:
  void create() override;
  void update() override;

  void onButtonClick() override;
  void onButtonLongPress() override;

 private:
  lv_obj_t* settingsPanel;
  lv_obj_t* settingsLabel;
};

#endif  // THEME_SETTINGS_PAGE_H
