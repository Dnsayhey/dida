#include "init_page.h"

void InitPage::create() {
  pageObject = createTransparentPage();
  statusPanel = createCardPanel(212, 128);
  lv_obj_align(statusPanel, LV_ALIGN_CENTER, 0, 0);

  statusLabel = createTextLabel(statusPanel, "正在启动\n请稍候");
  lv_obj_set_style_text_align(statusLabel, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_width(statusLabel, LV_PCT(92));
  lv_obj_align(statusLabel, LV_ALIGN_CENTER, 0, 0);

  hide();  // 默认隐藏
}

void InitPage::update() {
  // 可以在这里更新初始化进度
}
