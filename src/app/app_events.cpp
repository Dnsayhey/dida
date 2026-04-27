#include "app_events.h"

const char* buttonEventName(ButtonEvent event) {
  switch (event) {
    case ButtonEvent::Click:
      return "Click";
    case ButtonEvent::DoubleClick:
      return "DoubleClick";
    case ButtonEvent::LongPress:
      return "LongPress";
    default:
      return "Unknown";
  }
}

const char* appCommandTypeName(AppCommandType type) {
  switch (type) {
    case AppCommandType::None:
      return "None";
    case AppCommandType::SwitchPage:
      return "SwitchPage";
    case AppCommandType::DispatchPageClick:
      return "DispatchPageClick";
    case AppCommandType::DispatchPageLongPress:
      return "DispatchPageLongPress";
    default:
      return "Unknown";
  }
}
