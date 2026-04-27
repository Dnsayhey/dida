#ifndef UI_THEME_H
#define UI_THEME_H

#include <lvgl.h>

struct UiThemeColors {
  lv_color_t background;
  lv_color_t cardBackground;
  lv_color_t text;
  lv_color_t border;
  lv_opa_t pageOpacity;
  lv_opa_t cardOpacity;
  lv_opa_t borderOpacity;
};

class UiTheme {
 public:
  static UiThemeColors current();
  static UiThemeColors light();
  static UiThemeColors dark();
};

#endif  // UI_THEME_H
