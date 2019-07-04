#ifndef WZP_THREAD_SIMPLE_THREAD_POOL_H_
#define WZP_THREAD_SIMPLE_THREAD_POOL_H_

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <atomic>


//------------------------------------------------------------------------------
// Simple ThreadPool that creates N threads upon its creation,
// and pulls from a queue to get new jobs.
// Basic Usage:
//
//   /* Create thread pool with 4 threads */
//   ThreadPool pool(4);
//   /* Enqueue and store future */
//   auto result = pool.enqueue([](int answer) { return answer; }, 42);
//   /* Get result from future*/
//   std::cout << result.get() << std::endl;
//
// This class requires a number of c++11 features be present in your compiler.
//------------------------------------------------------------------------------
namespace wzp
{

class ThreadPool {
 public:
  // Constructor and Destructor
  ThreadPool(size_t);
  ~ThreadPool();

  // Add task to current queue
  template<class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

  // Sync threads
  void Sync(int wait_count);

  // Return the number of threads
  size_t ThreadNumber();

private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;
    // the task queue
    std::queue<std::function<void()>> tasks;
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::mutex sync_mutex;
    std::condition_variable sync_condition;
    bool stop;
    std::atomic_int sync { 0 };
};

// The constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    : stop(false) {
  for(size_t i = 0; i<threads; ++i)
    workers.emplace_back(
      [this]
      {
        for(;;) {
            std::function<void()> task;
            {
              std::unique_lock<std::mutex> lock(this->queue_mutex);
              this->condition.wait(lock,
                [this]{ return this->stop || !this->tasks.empty(); });
              if (this->stop && this->tasks.empty()) {
                return;
              }
              task = std::move(this->tasks.front());
              this->tasks.pop();
            }
         task();
         sync++;
         {
           std::unique_lock<std::mutex> lock(this->sync_mutex);
           sync_condition.notify_one();
         }
      }
    }
  );
}

// Add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;
  auto task = std::make_shared< std::packaged_task<return_type()> >(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
  );
  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    // don't allow enqueueing after stopping the pool
    if (stop) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks.emplace([task](){ (*task)(); });
  }
  condition.notify_one();
  return res;
}

// Wait all thread to finish their jobs
inline void ThreadPool::Sync(int wait_count) {
  std::unique_lock<std::mutex> lock(sync_mutex);
  this->sync_condition.wait(lock, [&]() {
    return sync == wait_count;
  });
  sync = 0;
}

// Return the number of threads
inline size_t ThreadPool::ThreadNumber() {
  return workers.size();
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  condition.notify_all();
  for (std::thread &worker: workers) {
    worker.join();
  }
}

} //wzp

#endif  // WZP_THREAD_SIMPLE_THREAD_POOL_H_
