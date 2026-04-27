#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <lvgl.h>

#include "app/page_type.h"
#include "base_page.h"

class PageManager {
 public:
  static PageManager& getInstance();

  // 初始化页面管理器
  void init();

  // 更新当前页面
  void update();
  void applyTheme();

  // 页面切换
  void switchToPage(PageType page);

  // 获取当前页面
  PageType getCurrentPage() const { return currentPage; }
  bool isInitialized() const { return initialized; }

  // 当前页面内部按钮事件处理
  void dispatchClickToCurrentPage();
  void dispatchLongPressToCurrentPage();

  const char* getPageName(PageType page) const;

 private:
  PageManager();
  ~PageManager() = default;

  bool initialized;
  PageType currentPage;
  BasePage* pages[6];  // 页面指针数组
};

#endif  // PAGE_MANAGER_H
