#include <cstddef>
#include <iostream>
#include "shfl-threading.h"

using namespace shfl::threading;

int main() {
  size_t requested_workers = 0;
  Workers workers{requested_workers};
  std::mutex iostream_m;
  auto constexpr num_tasks = 100;
  t_latch task_counter(num_tasks);

  srand(time(0));

  auto task_id = 0;
  for (int i = 0; i != num_tasks; ++i) {
    workers.assign({Task::Cmd::run,
                    [&iostream_m, task_id,
                     &task_counter](std::vector<t_task_param> const&) {
                      int task_len_ms = (rand() % 101) * 10;
                      std::this_thread::sleep_for(
                          std::chrono::milliseconds{task_len_ms});
                      {
                        std::lock_guard cout_guard{iostream_m};
                        std::cout << "worker #" << std::this_thread::get_id()
                                  << " completed task #" << task_id << '\n';
                      }
                      task_counter.count_down();
                    },
                    {}});
    ++task_id;
  }

  task_counter.wait();  // wait for tasks to complete
  std::cout << num_tasks << " tasks completed by " << requested_workers
            << " workers" << std::endl;
}
