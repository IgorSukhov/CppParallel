#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "mytask.h"

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <numeric>

using namespace std;

class ThreadPool
{
public:
    ThreadPool(unsigned int threadsNum);
    ~ThreadPool();
    mutable mutex m;
    vector<thread> threads;
    queue<MyTask> tasks;
    bool stop = false;
    void task_thread();
    void addTask(MyTask &task);
    static unsigned int tasksDone;
};

#endif // THREADPOOL_H

