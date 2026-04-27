#ifndef APP_EVENTS_H
#define APP_EVENTS_H

#include "page_type.h"

enum class ButtonEvent {
  Click,
  DoubleClick,
  LongPress,
};

enum class AppCommandType {
  None,
  SwitchPage,
  DispatchPageClick,
  DispatchPageLongPress,
};

struct AppCommand {
  AppCommand() = default;
  AppCommand(AppCommandType commandType, PageType page)
      : type(commandType), targetPage(page) {}

  AppCommandType type = AppCommandType::None;
  PageType targetPage = PAGE_INIT;
};

const char* buttonEventName(ButtonEvent event);
const char* appCommandTypeName(AppCommandType type);

#endif  // APP_EVENTS_H
