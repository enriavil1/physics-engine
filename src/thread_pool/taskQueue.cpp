#include "./taskQueue.hpp"

template <typename TCallback> void TaskQueue::addTask(TCallback &&callback) {
  std::lock_guard<std::mutex> lock_guard{this->m_mutex};
  this->m_tasks.push(callback);
  ++this->m_remaining_tasks;
}

template <typename TCallBack> void TaskQueue::getTask(TCallBack &callback) {
  std::lock_guard<std::mutex> lock_guard{this->m_mutex};

  if (this->m_remaining_tasks == 0) {
    return;
  }

  callback = std::move(this->m_tasks.front());
  this->m_tasks.pop();
}

void TaskQueue::waitForCompletion() const {
  while (this->m_remaining_tasks > 0) {
    TaskQueue::wait();
  }
}

void TaskQueue::workDone() { --this->m_remaining_tasks; }
