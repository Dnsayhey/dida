#ifndef NETWORK_SETUP_PAGE_H
#define NETWORK_SETUP_PAGE_H

#include <Arduino.h>

#include "base_page.h"

class NetworkSetupPage : public BasePage {
 public:
  void create() override;
  void update() override;

 private:
  String getPortalSsid() const;
  String getPortalUrl() const;

  lv_obj_t* infoPanel;
  lv_obj_t* infoLabel;
};

#endif  // NETWORK_SETUP_PAGE_H
