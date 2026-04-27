#include "provisioning_portal.h"

#include <WiFi.h>

#include "board/board_config.h"
#include "esp_wifi.h"

namespace {

constexpr byte kDnsPort = 53;

}  // namespace

bool ProvisioningPortal::begin() {
  _ssid = createPortalSsid();
  _configured = false;
  scanWiFiOptions();

  WiFi.mode(WIFI_AP);
  bool ok = WiFi.softAP(_ssid.c_str(), BoardConfig::PROVISIONING_AP_PASSWORD);
  if (!ok) {
    Serial.println("[ProvisioningPortal] failed to start SoftAP");
    return false;
  }

  _ipAddress = WiFi.softAPIP();
  _dnsServer.start(kDnsPort, "*", _ipAddress);
  _server.on("/", HTTP_GET, [this]() { handleRoot(); });
  _server.on("/generate_204", HTTP_GET, [this]() { handleCaptiveProbe(); });
  _server.on("/gen_204", HTTP_GET, [this]() { handleCaptiveProbe(); });
  _server.on("/hotspot-detect.html", HTTP_GET,
             [this]() { handleCaptiveProbe(); });
  _server.on("/library/test/success.html", HTTP_GET,
             [this]() { handleCaptiveProbe(); });
  _server.on("/connecttest.txt", HTTP_GET, [this]() { handleCaptiveProbe(); });
  _server.on("/ncsi.txt", HTTP_GET, [this]() { handleCaptiveProbe(); });
  _server.on("/favicon.ico", HTTP_GET, [this]() { handleNoContent(); });
  _server.on("/save", HTTP_POST, [this]() { handleSave(); });
  _server.onNotFound([this]() { redirectToRoot(); });
  _server.begin();
  _running = true;

  Serial.printf("[ProvisioningPortal] SoftAP started: %s, ip=%s\n",
                _ssid.c_str(), _ipAddress.toString().c_str());
  return true;
}

void ProvisioningPortal::handleClient() {
  if (!_running) {
    return;
  }

  _dnsServer.processNextRequest();
  _server.handleClient();
}

void ProvisioningPortal::stop() {
  if (!_running) {
    return;
  }

  _server.stop();
  _dnsServer.stop();
  WiFi.softAPdisconnect(true);
  _running = false;
  Serial.println("[ProvisioningPortal] stopped");
}

String ProvisioningPortal::buildPortalPage(const String& message) const {
  String page;
  page.reserve(4200);
  page += F("<!doctype html><html lang=\"zh-CN\"><head>");
  page += F("<meta charset=\"utf-8\">");
  page += F("<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">");
  page += F("<title>DIDA 配网</title>");
  page += F("<style>");
  page += F("body{margin:0;font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;background:#f4efe5;color:#241b14;}");
  page += F(".wrap{max-width:420px;margin:0 auto;padding:28px 20px;}");
  page += F(".card{background:#fffaf0;border:1px solid #dfd0bd;border-radius:18px;padding:22px;box-shadow:0 12px 40px rgba(88,63,37,.14);}");
  page += F("h1{font-size:26px;margin:0 0 8px;}p{line-height:1.6;color:#6a5847;}label{display:block;margin-top:16px;font-weight:700;}");
  page += F("input,select{width:100%;box-sizing:border-box;margin-top:8px;border:1px solid #cdbda8;border-radius:12px;padding:13px;font-size:16px;background:white;}");
  page += F("button{width:100%;margin-top:22px;border:0;border-radius:14px;padding:14px;font-size:17px;font-weight:800;background:#2f6f4e;color:white;}");
  page += F(".msg{margin:14px 0;padding:10px 12px;border-radius:12px;background:#fff0d6;color:#7a4d00;}");
  page += F(".hint{font-size:13px;color:#8a7661;margin:8px 0 0;}");
  page += F("</style></head><body><main class=\"wrap\"><section class=\"card\">");
  page += F("<h1>DIDA 配网</h1>");
  page += F("<p>选择家庭 Wi-Fi 并填写天气位置。提交后设备会保存配置并继续启动。</p>");
  if (!message.isEmpty()) {
    page += F("<div class=\"msg\">");
    page += htmlEscape(message);
    page += F("</div>");
  }
  page += F("<form method=\"post\" action=\"/save\">");
  page += F("<label>Wi-Fi SSID</label>");
  page += F("<select name=\"ssid_select\" id=\"ssidSelect\">");
  page += F("<option value=\"\">手动输入 / 未找到我的 Wi-Fi</option>");
  for (int i = 0; i < _wifiOptionCount; ++i) {
    page += F("<option value=\"");
    page += htmlEscape(_wifiOptions[i]);
    page += F("\">");
    page += htmlEscape(_wifiOptions[i]);
    page += F("</option>");
  }
  page += F("</select>");
  page += F("<input name=\"ssid_manual\" id=\"ssidManual\" placeholder=\"手动输入 Wi-Fi 名称\" maxlength=\"64\">");
  page += F("<p class=\"hint\">如果列表里没有你的 Wi-Fi，可以直接手动输入。</p>");
  page += F("<label>Wi-Fi 密码</label><input name=\"password\" type=\"password\" required maxlength=\"64\">");
  page += F("<label>省市/上级区域</label><input name=\"adm\" placeholder=\"例如：杭州\" maxlength=\"32\">");
  page += F("<label>区县/位置</label><input name=\"location\" required placeholder=\"例如：余杭\" maxlength=\"32\">");
  page += F("<button id=\"submitButton\" type=\"submit\">保存并连接</button>");
  page += F("</form><script>");
  page += F("const s=document.getElementById('ssidSelect'),m=document.getElementById('ssidManual');");
  page += F("function sync(){m.required=!s.value;m.style.display=s.value?'none':'block';}");
  page += F("s.addEventListener('change',sync);sync();");
  page += F("document.querySelector('form').addEventListener('submit',()=>{const b=document.getElementById('submitButton');b.disabled=true;b.textContent='保存中...';});");
  page += F("</script></section></main></body></html>");
  return page;
}

void ProvisioningPortal::handleRoot() {
  _server.send(200, "text/html; charset=utf-8", buildPortalPage());
}

void ProvisioningPortal::handleSave() {
  DeviceConfigSnapshot config = {provisionedSsidFromForm(), formValue("password"),
                                 formValue("adm"), formValue("location")};

  config.wifiSSID.trim();
  config.adm.trim();
  config.location.trim();

  if (config.wifiSSID.isEmpty() || config.wifiPassword.isEmpty() ||
      config.location.isEmpty()) {
    _server.send(400, "text/html; charset=utf-8",
                 buildPortalPage("Wi-Fi SSID、密码和位置不能为空"));
    return;
  }

  _configProvider.saveProvisionedConfig(config);
  _configured = true;
  _server.send(200, "text/html; charset=utf-8",
               buildPortalPage("配置已保存，设备正在连接 Wi-Fi"));
  Serial.println("[ProvisioningPortal] config saved");
}

void ProvisioningPortal::handleCaptiveProbe() {
  redirectToRoot();
}

void ProvisioningPortal::handleNoContent() {
  _server.send(204, "text/plain", "");
}

void ProvisioningPortal::redirectToRoot() {
  _server.sendHeader("Location", "/", true);
  _server.send(302, "text/plain", "");
}

void ProvisioningPortal::scanWiFiOptions() {
  _wifiOptionCount = 0;
  WiFi.disconnect(true, false);
  delay(200);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);

  wifi_country_t country = {"CN", 1, 13, 20, WIFI_COUNTRY_POLICY_MANUAL};
  if (esp_wifi_set_country(&country) != ESP_OK) {
    Serial.println("[ProvisioningPortal] failed to set WiFi country CN");
  }

  int16_t networkCount = WiFi.scanNetworks(false, false);
  if (networkCount <= 0) {
    Serial.printf("[ProvisioningPortal] WiFi scan found no networks: %d\n",
                  networkCount);
    WiFi.scanDelete();
    return;
  }

  for (int16_t i = 0; i < networkCount && _wifiOptionCount < 12; ++i) {
    String scannedSsid = WiFi.SSID(i);
    scannedSsid.trim();
    if (scannedSsid.isEmpty()) {
      continue;
    }

    bool duplicate = false;
    for (int j = 0; j < _wifiOptionCount; ++j) {
      if (_wifiOptions[j] == scannedSsid) {
        duplicate = true;
        break;
      }
    }
    if (duplicate) {
      continue;
    }

    _wifiOptions[_wifiOptionCount++] = scannedSsid;
  }

  WiFi.scanDelete();
}

String ProvisioningPortal::formValue(const String& name) {
  return _server.hasArg(name) ? _server.arg(name) : "";
}

String ProvisioningPortal::provisionedSsidFromForm() {
  String selected = formValue("ssid_select");
  selected.trim();
  if (!selected.isEmpty()) {
    return selected;
  }

  return formValue("ssid_manual");
}

String ProvisioningPortal::htmlEscape(const String& value) const {
  String escaped;
  escaped.reserve(value.length());
  for (int i = 0; i < value.length(); ++i) {
    char c = value.charAt(i);
    switch (c) {
      case '&':
        escaped += F("&amp;");
        break;
      case '<':
        escaped += F("&lt;");
        break;
      case '>':
        escaped += F("&gt;");
        break;
      case '"':
        escaped += F("&quot;");
        break;
      default:
        escaped += c;
        break;
    }
  }
  return escaped;
}

String ProvisioningPortal::createPortalSsid() const {
  uint64_t chipId = ESP.getEfuseMac();
  char suffix[5];
  snprintf(suffix, sizeof(suffix), "%04X",
           static_cast<unsigned int>(chipId & 0xFFFF));
  return String("DIDA-") + suffix;
}
