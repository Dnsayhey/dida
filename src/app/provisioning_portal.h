#ifndef PROVISIONING_PORTAL_H
#define PROVISIONING_PORTAL_H

#include <Arduino.h>
#include <DNSServer.h>
#include <WebServer.h>

#include "device_config_provider.h"

class ProvisioningPortal {
 public:
  bool begin();
  void handleClient();
  void stop();
  bool isConfigured() const { return _configured; }
  String getSsid() const { return _ssid; }
  IPAddress getIpAddress() const { return _ipAddress; }

 private:
  String buildPortalPage(const String& message = "") const;
  void handleRoot();
  void handleSave();
  void handleCaptiveProbe();
  void handleNoContent();
  void redirectToRoot();
  void scanWiFiOptions();
  String formValue(const String& name);
  String provisionedSsidFromForm();
  String htmlEscape(const String& value) const;
  String createPortalSsid() const;

  DNSServer _dnsServer;
  WebServer _server{80};
  DeviceConfigProvider _configProvider;
  String _ssid;
  String _wifiOptions[12];
  IPAddress _ipAddress{192, 168, 4, 1};
  int _wifiOptionCount = 0;
  bool _running = false;
  bool _configured = false;
};

#endif  // PROVISIONING_PORTAL_H
