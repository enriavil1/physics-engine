#pragma once

#include <iostream>
#include <ostream>
#include <thread>
#include <vector>

#include "taskQueue.hpp"
#include "worker.hpp"

typedef std::vector<Worker *> workers;

class ThreadPool {
private:
  uint32_t m_thread_count = 0;
  TaskQueue m_task_queue;
  workers m_workers;

public:
  ThreadPool(uint32_t thread_count) : m_thread_count{thread_count} {
    // the vector of workers should have a max size of thread count
    this->m_workers.reserve(thread_count);
    for (uint32_t i = 0; i < thread_count; i++) {
      this->m_workers.push_back(new Worker(this->m_task_queue, i));
    }
  }

  virtual ~ThreadPool() {
    // need to stop all workers running
    for (Worker *worker : this->m_workers) {
      worker->stop();
    }
  }

  uint32_t getThreadCount() { return this->m_thread_count; }

  template <typename TCallback> void addTask(TCallback&& callback) {
    this->m_task_queue.addTask(std::forward<TCallback>(callback));
  }

  void waitForCompletion() const { this->m_task_queue.waitForCompletion(); }
};
