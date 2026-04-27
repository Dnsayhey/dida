#include "network_setup_page.h"

#include <WiFi.h>

#include "app/app_controller.h"
#include "board/board_config.h"

void NetworkSetupPage::create() {
  pageObject = createTransparentPage();
  infoPanel = createCardPanel(224, 210);
  lv_obj_align(infoPanel, LV_ALIGN_CENTER, 0, 0);

  infoLabel = createTextLabel(infoPanel, "");
  lv_obj_set_style_text_align(infoLabel, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_width(infoLabel, LV_PCT(92));
  lv_obj_align(infoLabel, LV_ALIGN_CENTER, 0, 0);
  update();

  hide();  // 默认隐藏
}

void NetworkSetupPage::update() {
  if (AppController::getInstance().getState() == AppState::ConnectingWifi) {
    lv_label_set_text(infoLabel, "正在连接 WiFi\n请稍候...");
    return;
  }

  String text = "请先连接到 WiFi:\n" + getPortalSsid() + "\n\n密码:\n" +
                BoardConfig::PROVISIONING_AP_PASSWORD + "\n\n浏览器访问:\n" +
                getPortalUrl();
  lv_label_set_text(infoLabel, text.c_str());
}

String NetworkSetupPage::getPortalSsid() const {
  String ssid = WiFi.softAPSSID();
  return ssid.isEmpty() ? "DIDA" : ssid;
}

String NetworkSetupPage::getPortalUrl() const {
  IPAddress ip = WiFi.softAPIP();
  String ipText = ip.toString();
  return ipText == "0.0.0.0" ? "192.168.4.1" : ipText;
}
