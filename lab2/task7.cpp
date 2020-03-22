#include "task7.h"
#include "threadpool.h"
#include <iostream>


void showTime7(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point stop)
{
    chrono::duration<double> time_elapsed = stop - start;
    cout << chrono::duration_cast<chrono::microseconds>(time_elapsed).count() << " us" << endl;
}

void Task7()
{
    chrono::high_resolution_clock::time_point start, stop;
    start = chrono::high_resolution_clock::now();
    vector<MyTask> tasks;
    tasks.reserve(20);
    ThreadPool tp(1);
    for(unsigned long i=0;i<20;i++)
    {
        tasks.emplace_back();
        tp.addTask(tasks[i]);
    }

    stop = chrono::high_resolution_clock::now();
    cout << "for_each time: ";
    showTime7(start,stop);
    this_thread::sleep_for(chrono::seconds(5));
}
