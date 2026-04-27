#include "base_page.h"

#include "sources/font/ui_zh_20.c"
#include "ui_theme.h"

lv_obj_t* BasePage::createTransparentPage() {
  lv_obj_t* page = lv_obj_create(lv_scr_act());
  lv_obj_set_size(page, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_opa(page, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_opa(page, LV_OPA_0, 0);
  lv_obj_align(page, LV_ALIGN_CENTER, 0, 0);
  return page;
}

lv_obj_t* BasePage::createTextLabel(lv_obj_t* parent, const char* text) {
  lv_obj_t* label = lv_label_create(parent);
  applyUiTextFont(label);
  applyThemeToNode(label);
  lv_label_set_text(label, text);
  return label;
}

lv_obj_t* BasePage::createCenteredLabel(const char* text) {
  lv_obj_t* label = createTextLabel(pageObject, text);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_width(label, LV_PCT(90));
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  return label;
}

lv_obj_t* BasePage::createCardPanel(lv_coord_t width, lv_coord_t height) {
  lv_obj_t* panel = lv_obj_create(pageObject);
  lv_obj_set_size(panel, width, height);
  lv_obj_set_style_border_width(panel, 1, 0);
  lv_obj_set_style_radius(panel, 12, 0);
  markThemeCard(panel);
  applyThemeToNode(panel);
  return panel;
}

void BasePage::markThemeCard(lv_obj_t* node) {
  lv_obj_add_flag(node, LV_OBJ_FLAG_USER_1);
}

void BasePage::applyUiTextFont(lv_obj_t* label) {
  lv_obj_set_style_text_font(label, &ui_zh_20, 0);
}

void BasePage::applyTheme() {
  if (pageObject) {
    applyThemeToNode(pageObject);
  }
}

void BasePage::applyThemeToNode(lv_obj_t* node) {
  if (!node) {
    return;
  }

  UiThemeColors theme = UiTheme::current();
  bool isThemeCard = lv_obj_has_flag(node, LV_OBJ_FLAG_USER_1);
  if (node == pageObject) {
    lv_obj_set_style_bg_opa(node, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(node, LV_OPA_TRANSP, 0);
  } else if (lv_obj_check_type(node, &lv_label_class) && !isThemeCard) {
    lv_obj_set_style_text_color(node, theme.text, 0);
    lv_obj_set_style_bg_opa(node, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(node, LV_OPA_TRANSP, 0);
  } else {
    lv_obj_set_style_bg_color(node, theme.cardBackground, 0);
    lv_obj_set_style_bg_opa(node, theme.cardOpacity, 0);
    lv_obj_set_style_border_color(node, theme.border, 0);
    lv_obj_set_style_border_opa(node, theme.borderOpacity, 0);
  }

  uint32_t childCount = lv_obj_get_child_count(node);
  for (uint32_t i = 0; i < childCount; ++i) {
    applyThemeToNode(lv_obj_get_child(node, i));
  }
}
