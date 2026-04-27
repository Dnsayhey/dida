#ifndef BACKLIGHT_CONTROLLER_H
#define BACKLIGHT_CONTROLLER_H

#include "hardware/backlight.h"

class BacklightController {
 public:
  BrightnessMode currentMode() const;
  BrightnessMode switchToNextMode();
};

#endif  // BACKLIGHT_CONTROLLER_H
