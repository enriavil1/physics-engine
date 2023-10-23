#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <thread>

typedef std::function<void()> task;

class TaskQueue {
private:
  std::queue<task> m_tasks;
  std::mutex m_mutex;
  std::atomic<uint32_t> m_remaining_tasks = 0;

public:
  static void wait() { std::this_thread::yield(); }

  template <typename TCallback> void addTask(TCallback &&callback);

  template <typename TCallback> void getTask(TCallback &callback);

  void waitForCompletion() const;

  void workDone();
};
