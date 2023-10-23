#pragma once

#include <functional>
#include <thread>

#include "taskQueue.hpp"

class Worker {
private:
  uint32_t m_id = 0;
  bool m_is_running = true;
  std::thread m_thread;

  std::function<void()> m_task = nullptr;
  TaskQueue *m_task_queue = nullptr;

public:
  Worker(TaskQueue& task_queue, uint32_t id);

  void run();
  void stop();
};
