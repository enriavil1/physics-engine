#include "./task_queue.hpp"

template <typename TCallBack> void TaskQueue::addTask(TCallBack&& callback) {
  std::lock_guard<std::mutex> lock_guard{this->m_mutex};
  this->m_tasks.push(callback);
  ++this->m_remaining_tasks;
}

template <typename TCallBack> void TaskQueue::getTask(TCallBack& callback) {
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
