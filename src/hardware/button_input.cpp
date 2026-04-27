#include "button_input.h"

void ButtonInput::begin(uint8_t pin, EventHandler handler) {
  _handler = handler;
  _button.setup(pin, INPUT_PULLUP, true);

  _button.attachClick(
      [](void* context) {
        static_cast<ButtonInput*>(context)->_handler(ButtonEvent::Click);
      },
      this);

  _button.attachDoubleClick(
      [](void* context) {
        static_cast<ButtonInput*>(context)->_handler(ButtonEvent::DoubleClick);
      },
      this);

  _button.attachLongPressStart(
      [](void* context) {
        static_cast<ButtonInput*>(context)->_handler(ButtonEvent::LongPress);
      },
      this);
}

void ButtonInput::update() { _button.tick(); }
