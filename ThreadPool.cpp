#include "ThreadPool.h"

ThreadPool::ThreadPool(int n) {

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