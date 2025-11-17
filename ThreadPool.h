#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>


using namespace std;

class ThreadPool {
	vector<thread> threads;
	queue<function<void()>> tasks;
	condition_variable cv;
	mutex mtx;
	bool stop = false;

public:
	ThreadPool(int n = thread::hardware_concurrency());

	void emplace(function<void()>&& task);

	~ThreadPool();
};


#endif //THREADPOOL_H