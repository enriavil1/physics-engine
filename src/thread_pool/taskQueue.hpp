#pragma once

#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdint.h>
#include <thread>

typedef std::function<void()> task;

class TaskQueue {
private:
  std::queue<task> m_tasks;
  std::mutex m_mutex;

  std::atomic<uint32_t> m_tasks_remaining = 0;

public:
  static void wait() { std::this_thread::yield(); }

  template <typename TCallback> void addTask(TCallback&& callback) {
    std::lock_guard<std::mutex> lock_guard{this->m_mutex};
    this->m_tasks.push(std::forward<TCallback>(callback));
    this->m_tasks_remaining++;
  }

  template <typename TCallback> void getTask(TCallback& callback) {
    {
      std::lock_guard<std::mutex> lock_guard{this->m_mutex};

      if (this->m_tasks.empty()) {
        return;
      }

      callback = std::move(this->m_tasks.front());
      this->m_tasks.pop();
    }
  }

  void markTaskFinished() { this->m_tasks_remaining--; }

  void waitForCompletion() const {
    while (this->m_tasks_remaining > 0) {
      TaskQueue::wait();
    }
  }
};
