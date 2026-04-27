#include "backlight_controller.h"

BrightnessMode BacklightController::currentMode() const {
  return Backlight::getInstance().currentMode();
}

BrightnessMode BacklightController::switchToNextMode() {
  return Backlight::getInstance().nextMode();
}
