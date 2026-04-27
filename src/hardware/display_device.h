#ifndef DISPLAY_DEVICE_H
#define DISPLAY_DEVICE_H

#include <Arduino.h>
#include <freertos/semphr.h>

class DisplayDevice {
 public:
  bool begin();
  bool lock(TickType_t timeoutTicks);
  bool lockForMs(uint32_t timeoutMs);
  void unlock();
  void handleTimers();

 private:
  SemaphoreHandle_t _guiMutex = nullptr;
};

#endif  // DISPLAY_DEVICE_H
