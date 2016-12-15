#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <iostream>
#include <thread>
#include <atomic>
#include <future>
#include <functional>
#include <vector>
#include <memory>

#include "container/threadsafe_queue.h"


namespace wzp{
    /*manage the threads(vector<thread>)*/
	class join_threads {
	private:
		std::vector<std::thread>& threads;
	public:
		explicit join_threads(std::vector <std::thread>& threads_) :threads(threads_) {}
		~join_threads() {
			decltype(threads.size()) i = 0;
			for (; i < threads.size(); ++i) {
				if (threads[i].joinable()) {
					threads[i].join();
				}
			}
			std::cout<<"END"<<std::endl;
		}
	};
	/*the thread pool class*/
	class thread_pool {
	private:
		using Task = std::function<void()>;
		std::atomic<bool> done;
		threadsafe_queue<Task> task_queue;
		std::vector<std::thread> threads;
		join_threads join_threads_;
	private:
		/*pop one task or sleep for each thread*/
		void work_thread() {
			while (!done) {
				Task task;
				task_queue.wait_and_pop(task);
				task();
			}
		}
	public:
		thread_pool(const int pool_num = -1)
			:done(false),
			threads(),
			join_threads_(threads) {
			auto num = 0;
			if (pool_num == -1) {
				num = std::thread::hardware_concurrency();
			}
			else {
				num = pool_num;
			}
			try {
				for (auto i = 0; i < num; ++i) {
					threads.emplace_back(&thread_pool::work_thread, this);
				}
			}
			catch (...) {
				done = true;
				throw;
			}
		}

		~thread_pool(){
			done = true;
		}

		template<typename F>
		void submit(F&& f) {
			task_queue.push(Task(f));
		}

		void done_work() {
			done = true;
		}

		//give a loop interface
		void loop() {
			for (size_t i = 0; i < threads.size(); ++i) {
				if (threads[i].joinable()) {
					threads[i].join();
				}
			}
		}

	};
}

using ThreadPoolPtr = std::unique_ptr<wzp::thread_pool>;

#endif /*THREAD_POOL_H_*/
