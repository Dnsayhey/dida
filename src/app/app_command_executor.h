#ifndef APP_COMMAND_EXECUTOR_H
#define APP_COMMAND_EXECUTOR_H

#include "app_events.h"

class PageManager;

class AppCommandExecutor {
 public:
  void execute(const AppCommand& command, PageManager& pageManager);
};

#endif  // APP_COMMAND_EXECUTOR_H
