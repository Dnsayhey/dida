#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <Arduino.h>

enum BrightnessMode { AUTO, LEVEL_1, LEVEL_2, LEVEL_3 };

class Backlight {
 public:
  static Backlight& getInstance();

  void begin(uint8_t pin, BrightnessMode initialMode = AUTO);
  BrightnessMode nextMode();
  void setMode(BrightnessMode mode);

  BrightnessMode currentMode() const { return _mode; }

  void setBrightness(uint8_t brightness);
  void update();

 private:
  Backlight() = default;
  Backlight(const Backlight&) = delete;
  Backlight& operator=(const Backlight&) = delete;

  BrightnessMode _mode = AUTO;
  uint8_t _pin;
  uint8_t _levels[4] = {128, 80, 160, 255};
};

#endif
