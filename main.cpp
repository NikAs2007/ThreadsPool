#include "ThreadPool.h"

#include <iostream>

int main()
{
    ThreadPool thread_pool;

    thread_pool.emplace([]() { cout << "Hi"; });

    return 0;
}