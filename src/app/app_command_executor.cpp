#include "app_command_executor.h"

#include <Arduino.h>

#include "pages/page_manager.h"

void AppCommandExecutor::execute(const AppCommand& command,
                                 PageManager& pageManager) {
  Serial.printf("[AppCommandExecutor] execute %s\n",
                appCommandTypeName(command.type));

  switch (command.type) {
    case AppCommandType::SwitchPage:
      pageManager.switchToPage(command.targetPage);
      break;
    case AppCommandType::DispatchPageClick:
      pageManager.dispatchClickToCurrentPage();
      break;
    case AppCommandType::DispatchPageLongPress:
      pageManager.dispatchLongPressToCurrentPage();
      break;
    case AppCommandType::None:
      break;
  }
}
