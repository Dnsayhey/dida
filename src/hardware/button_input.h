#ifndef BUTTON_INPUT_H
#define BUTTON_INPUT_H

#include <Arduino.h>
#include <OneButton.h>

#include <functional>

#include "app/app_events.h"

class ButtonInput {
 public:
  using EventHandler = std::function<void(ButtonEvent)>;

  void begin(uint8_t pin, EventHandler handler);
  void update();

 private:
  OneButton _button;
  EventHandler _handler;
};

#endif  // BUTTON_INPUT_H
