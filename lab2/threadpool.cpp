#include "threadpool.h"

unsigned int ThreadPool::tasksDone = 0;

ThreadPool::ThreadPool(unsigned int threadsNum)
{
    for(unsigned int i = 0; i < threadsNum; ++i)
    {
        threads.reserve(threadsNum);
        threads.emplace_back(&ThreadPool::task_thread, this);
    }
}

ThreadPool::~ThreadPool()
{
    stop = true;
    for(thread& t : threads)
    {
        t.join();
    }
}

void ThreadPool::task_thread()
{
    while(!stop)
    {
        MyTask task;
        m.lock();
        if(tasks.empty())
        {
            m.unlock();
            this_thread::yield();
        }
        else
        {
            task = tasks.front();
            tasks.pop();
            m.unlock();
            task.taskToDo();
        }
    }
}

void ThreadPool::addTask(MyTask &task)
{
    lock_guard<mutex> lock(m);
    tasks.push(task);
}



