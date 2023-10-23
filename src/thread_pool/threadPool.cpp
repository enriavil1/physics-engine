#include "./threadPool.hpp"

ThreadPool::ThreadPool(uint32_t thread_count) : m_thread_count{thread_count} {
  // the vector of workers should have a max size of thread count
  this->m_workers.reserve(thread_count);
  for (uint32_t i = 0; i < thread_count; i++) {
    this->m_workers.push_back(Worker(this->m_task_queue, i));
  }
}

ThreadPool::~ThreadPool() {
  // need to stop all workers running
  for (Worker& worker : this->m_workers) {
    worker.stop();
  }
}

template <typename TCallback> void ThreadPool::addTask(TCallback&& callback) {
  this->m_task_queue.addTask(std::forward<TCallback>(callback));
}

void ThreadPool::waitForCompletion() const {
  this->m_task_queue.waitForCompletion();
}
