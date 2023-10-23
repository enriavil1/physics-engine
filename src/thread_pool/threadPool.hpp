#pragma once

#include <thread>
#include <vector>

#include "task_queue.hpp"
#include "worker.hpp"

typedef std::vector<Worker> workers;

class ThreadPool {
private:
  uint32_t m_thread_count = 0;
  TaskQueue m_task_queue;
  workers m_workers;

public:
  ThreadPool(uint32_t thread_count);
  virtual ~ThreadPool();

  template <typename TCallback> void addTask(TCallback&& callback);
  void waitForCompletion() const;
};
