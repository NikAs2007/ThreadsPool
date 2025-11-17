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
	
}