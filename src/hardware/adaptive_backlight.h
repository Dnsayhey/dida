#ifndef ADAPTIVE_BACKLIGHT_H
#define ADAPTIVE_BACKLIGHT_H

#include <Arduino.h>

#include "hardware/backlight.h"

class AdaptiveBacklight {
 public:
  void begin(uint8_t backlightPin, uint8_t lightSensorPin,
             BrightnessMode initialMode);
  void registerTasks();
  void update();
};

#endif  // ADAPTIVE_BACKLIGHT_H
