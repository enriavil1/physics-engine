#include "./worker.hpp"
#include <iostream>

Worker::Worker(TaskQueue& task_queue, uint32_t id)
    : m_id{id}, m_task_queue{&task_queue} {
  this->m_thread = std::thread([this]() { this->run(); });
}

void Worker::run() {
  while (this->m_is_running) {
    this->m_task_queue->getTask(this->m_task);
    if (this->m_task == nullptr) {
      // there arent any task in queue so we wait for a task
      TaskQueue::wait();
    } else {
      this->m_task();
      this->m_task = nullptr;
    }
  }
}

void Worker::stop() {
  this->m_is_running = false;
  this->m_thread.join();
}
