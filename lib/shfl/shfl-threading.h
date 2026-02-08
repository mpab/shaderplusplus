#pragma once

#include <condition_variable>
#include <functional>
#include <latch>
#include <mutex>
#include <queue>
#include <thread>
#include <variant>

namespace shfl {
namespace threading {

using t_task_param = std::variant<int, float, std::string>;

using t_latch = std::latch;

struct Task {
  enum Cmd { run, stop };
  Cmd cmd;
  std::function<void(std::vector<t_task_param>)> task_fn;
  std::vector<t_task_param> task_args;
};

template <typename T>
class ThreadSafeQueue {
 public:
  void push(T const& val) {
    std::lock_guard<std::mutex> q_lock{_q_synch};
    _q.push(val);
    _q_cond.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> q_lock{_q_synch};
    _q_cond.wait(q_lock, [&] { return !_q.empty(); });

    T ret = _q.front();
    _q.pop();
    return ret;
  }

 private:
  std::queue<T> _q;
  std::condition_variable _q_cond;
  std::mutex _q_synch;
};

inline auto new_thread(ThreadSafeQueue<Task>& ts_queue) {
  return std::thread{[&ts_queue] {
    while (true) {
      auto const elem = ts_queue.pop();
      switch (elem.cmd) {
        case Task::Cmd::run:
          elem.task_fn(elem.task_args);
          break;
        case Task::Cmd::stop:
          return;
      }
    }
  }};
}

class Workers {
 public:
  Workers(std::size_t& n_requested) {
    if (n_requested < 1) {
      n_requested = std::thread::hardware_concurrency();
    }
    for (std::size_t i = 0; i != n_requested; ++i) {
      _threads.push_back(new_thread(_ts_queue));
    }
  }

  ~Workers() {
    stop_all();
    wait_for_all();
  }

  bool assign(Task const& task) {
    _ts_queue.push(task);
    return true;
  }

  void stop_all() {
    Task const stop_task{Task::Cmd::stop, {}, {}};
    for (std::size_t i = 0; i != _threads.size(); ++i) {
      assign(stop_task);
    }
  }

  void wait_for_all() {
    for (auto it = _threads.begin(); it != _threads.end(); ++it) {
      it->join();
    }
  }

 private:
  ThreadSafeQueue<Task> _ts_queue;
  std::vector<std::thread> _threads;
};

}  // namespace threading
}  // namespace shfl
