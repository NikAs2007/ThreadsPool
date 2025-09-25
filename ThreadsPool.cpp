
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <future>
#include <mutex>
#include <functional>
#include <chrono>
using namespace std;

class ThreadsPool {
    vector<thread> threads;
    queue<function<void()>> tasks;
    mutex mtx;

    bool stop = false;

    void worker() {
        while (true) {
            function<void()> executable_func;
            mtx.lock();
            while (!stop && tasks.empty()) {
                mtx.unlock();
                this_thread::yield();  // Отдаем квант времени
                mtx.lock();
            }
            if (stop && tasks.empty()) {
                mtx.unlock();
                return;
            }
            executable_func = move(tasks.front());
            tasks.pop();
            mtx.unlock();
            executable_func();
        }
    }
public:
    ThreadsPool(int pools = thread::hardware_concurrency()) {
        threads.reserve(pools);
        for (int i = 0; i < pools; i++) {
            threads.emplace_back(thread(move([this]() { worker(); })));
        }
    }

    ~ThreadsPool() {
        mtx.lock();
        stop = true;
        mtx.unlock();
        for (thread& th : threads) {
            if (th.joinable()) th.join();
        }
    }

    void push_back(function<void()> func) {
        mtx.lock();
        tasks.emplace(move(func));
        mtx.unlock();
    }

};

int main()
{
    ThreadsPool pool;

    //pool.push_back([]() {
    //    cout << "Start from " << this_thread::get_id() << endl;
    //    this_thread::sleep_for(chrono::milliseconds(2000));
    //    cout << "End from " << this_thread::get_id() << endl;
    //    });

    //this_thread::sleep_for(chrono::milliseconds(500));

    //pool.push_back([]() {
    //    cout << "Start from " << this_thread::get_id() << endl;
    //    this_thread::sleep_for(chrono::milliseconds(2000));
    //    cout << "End from " << this_thread::get_id() << endl;
    //    });


    //this_thread::sleep_for(chrono::milliseconds(500));

    //pool.push_back([]() {
    //    cout << "Start from " << this_thread::get_id() << endl;
    //    this_thread::sleep_for(chrono::milliseconds(2000));
    //    cout << "End from " << this_thread::get_id() << endl;
    //    });

    return 0;
}