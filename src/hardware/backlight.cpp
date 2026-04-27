#include "backlight.h"

Backlight& Backlight::getInstance() {
  static Backlight instance;
  return instance;
}

void Backlight::begin(uint8_t pin, BrightnessMode initialMode) {
  _pin = pin;
  _mode = initialMode;
  analogWriteFrequency(5000);
}

BrightnessMode Backlight::nextMode() {
  _mode = static_cast<BrightnessMode>((static_cast<int>(_mode) + 1) % 4);
  update();
  return _mode;
}

void Backlight::setMode(BrightnessMode mode) {
  _mode = mode;
  update();
}

void Backlight::setBrightness(uint8_t brightness) {
  _levels[static_cast<int>(AUTO)] = brightness;
}

void Backlight::update() {
  analogWrite(_pin, _levels[static_cast<int>(_mode)]);
}
