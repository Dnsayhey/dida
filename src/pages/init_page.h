#ifndef INIT_PAGE_H
#define INIT_PAGE_H

#include "base_page.h"

class InitPage : public BasePage {
 public:
  void create() override;
  void update() override;

 private:
  lv_obj_t* statusPanel;
  lv_obj_t* statusLabel;
};

#endif  // INIT_PAGE_H
