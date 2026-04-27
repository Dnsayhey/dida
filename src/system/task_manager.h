#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H
#include <Arduino.h>

#include <vector>

struct Task {
  std::function<void()> callback;  // 任务回调函数
  unsigned long interval;          // 执行间隔（毫秒）
  unsigned long lastRun;           // 上次执行时间
  bool enabled;                    // 是否启用
  const char* name;                // 任务名称（用于调试）

  Task(std::function<void()> cb, unsigned long intervalMs, const char* taskName)
      : callback(cb),
        interval(intervalMs),
        lastRun(0),
        enabled(true),
        name(taskName) {}
};

class TaskManager {
 public:
  static TaskManager& getInstance();

  // 添加任务
  void addTask(std::function<void()> callback, unsigned long intervalMs,
               const char* name, bool runNow = false);

  // 运行任务管理器（在loop中调用）
  void run();

  // 启用/禁用任务
  void enableTask(const char* name, bool enable = true);

  // 获取任务状态
  void printTaskStatus();

  // 清除所有任务
  void clearTasks();

 private:
  std::vector<Task> tasks;
  TaskManager() = default;

  void runTask(Task& task);

  // 禁止拷贝构造和赋值
  TaskManager(const TaskManager&) = delete;
  TaskManager& operator=(const TaskManager&) = delete;
};

#endif  // TASK_MANAGER_H
