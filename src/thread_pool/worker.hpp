#pragma once

#include <functional>
#include <thread>

#include "taskQueue.hpp"

class Worker {
private:
  std::thread m_thread;

  uint32_t m_id = 0;
  bool m_is_running = true;

  std::function<void()> m_task = nullptr;
  TaskQueue *m_task_queue = nullptr;

public:
  Worker(TaskQueue& task_queue, uint32_t id)
      : m_id{id}, m_task_queue{&task_queue} {
    this->m_thread = std::thread([this]() { this->run(); });
  }

  void run() {
    while (this->m_is_running) {
      this->m_task_queue->getTask(this->m_task);
      if (this->m_task == nullptr) {
        // there arent any task in queue so we wait for a task
        TaskQueue::wait();
      } else {
        this->m_task();
        this->m_task_queue->markTaskFinished();
        this->m_task = nullptr;
      }
    }
  }

  void stop() {
    this->m_is_running = false;
    this->m_thread.join();
  }
};
