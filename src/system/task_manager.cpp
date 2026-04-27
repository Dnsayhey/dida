#include "task_manager.h"

#include "esp32-hal.h"

TaskManager& TaskManager::getInstance() {
  static TaskManager instance;
  return instance;
}

void TaskManager::addTask(std::function<void()> callback,
                          unsigned long intervalMs, const char* name,
                          bool runNow) {
  tasks.emplace_back(callback, intervalMs, name);
  Task& task = tasks.back();

  if (runNow) {
    Serial.printf("[TaskManager] Task added: %s (interval: %lu ms, runNow)\n",
                  name, intervalMs);
    runTask(task);
  } else {
    Serial.printf("[TaskManager] Task added: %s (interval: %lu ms)\n", name,
                  intervalMs);
  }
}

void TaskManager::run() {
  for (auto& task : tasks) {
    if (!task.enabled) continue;

    if (millis() - task.lastRun >= task.interval) {
      runTask(task);
    }
  }
}

void TaskManager::runTask(Task& task) {
  try {
    task.callback();
    task.lastRun = millis();
  } catch (...) {
    Serial.printf("[TaskManager] Error executing task: %s\n", task.name);
  }
}

void TaskManager::enableTask(const char* name, bool enable) {
  for (auto& task : tasks) {
    if (strcmp(task.name, name) == 0) {
      task.enabled = enable;
      Serial.printf("[TaskManager] Task %s: %s\n", name,
                    enable ? "enabled" : "disabled");
      return;
    }
  }
  Serial.printf("[TaskManager] Task not found: %s\n", name);
}

void TaskManager::printTaskStatus() {
  Serial.println("[TaskManager] === Task Status ===");
  for (const auto& task : tasks) {
    unsigned long elapsed = millis() - task.lastRun;
    unsigned long timeToNext =
        elapsed >= task.interval ? 0 : task.interval - elapsed;

    Serial.printf("  %s: %s (interval: %lu ms, next in: %lu ms)\n", task.name,
                  task.enabled ? "enabled" : "disabled", task.interval,
                  timeToNext);
  }
  Serial.println("[TaskManager] ===================");
}

void TaskManager::clearTasks() {
  tasks.clear();
  Serial.println("[TaskManager] All tasks cleared");
}
