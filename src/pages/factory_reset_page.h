#ifndef FACTORY_RESET_PAGE_H
#define FACTORY_RESET_PAGE_H

#include "base_page.h"

class FactoryResetPage : public BasePage {
 public:
  void create() override;
  void update() override;
  void onButtonLongPress() override;

 private:
  lv_obj_t* resetPanel;
  lv_obj_t* resetLabel;
  bool resetDone = false;
};

#endif  // FACTORY_RESET_PAGE_H
