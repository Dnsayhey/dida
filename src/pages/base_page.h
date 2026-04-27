#ifndef BASE_PAGE_H
#define BASE_PAGE_H

#include <lvgl.h>

class BasePage {
 public:
  BasePage() : pageObject(nullptr) {}
  virtual ~BasePage() = default;

  // 创建页面
  virtual void create() = 0;

  // 显示页面
  virtual void show() {
    if (pageObject) {
      lv_obj_clear_flag(pageObject, LV_OBJ_FLAG_HIDDEN);
    }
  }

  // 隐藏页面
  virtual void hide() {
    if (pageObject) {
      lv_obj_add_flag(pageObject, LV_OBJ_FLAG_HIDDEN);
    }
  }

  // 更新页面内容
  virtual void update() = 0;
  virtual void applyTheme();

  // 获取页面对象
  lv_obj_t* getPageObject() const { return pageObject; }

  // 按钮事件处理（由页面管理模块调用）
  virtual void onButtonClick() {}
  virtual void onButtonDoubleClick() {}
  virtual void onButtonLongPress() {}

 protected:
  lv_obj_t* createTransparentPage();
  lv_obj_t* createTextLabel(lv_obj_t* parent, const char* text = "");
  lv_obj_t* createCenteredLabel(const char* text = "");
  lv_obj_t* createCardPanel(lv_coord_t width, lv_coord_t height);
  void markThemeCard(lv_obj_t* node);
  void applyUiTextFont(lv_obj_t* label);
  void applyThemeToNode(lv_obj_t* node);

  lv_obj_t* pageObject;
};

#endif  // BASE_PAGE_H
