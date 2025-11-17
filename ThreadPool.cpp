#include "ThreadPool.h"

ThreadPool::ThreadPool(int n) {
	for (int i = 0; i < n; ++i) {
		threads.emplace_back([this]() {
			while (true) {
				function<void()> task;

				{
					unique_lock<mutex> lock(mtx);
					cv.wait(lock, [this]() { return stop || !tasks.empty(); });

					if (stop && tasks.empty()) {
						return;
					}

					task = move(tasks.front());
					tasks.pop();

				}

				task();
			}

			});
	}
}

void ThreadPool::emplace(function<void()>&& task) {
	{
		lock_guard<mutex> lock(mtx);
		tasks.push(task);
	}
	cv.notify_one();
}

ThreadPool::~ThreadPool() {
	{
		lock_guard<mutex> lock(mtx);
		stop = true;
	}
	cv.notify_all();
	for (int i = 0; i < threads.size(); ++i) {
		threads[i].join();
	}
}