#include "task3.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <iostream>
#include <thread>
#include <future>

using namespace std;

void showTime3(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point stop)
{
    chrono::duration<double> time_elapsed = stop - start;
    cout << chrono::duration_cast<chrono::microseconds>(time_elapsed).count() << " us" << endl;
}

int parallel_sum(vector<int> &v, unsigned int threadNum)
{

    long perThread = static_cast<long>(v.size()/(threadNum));

    vector<future<int>> vf;
    vf.reserve(threadNum);

    for(unsigned int i = 0; i < threadNum-0; ++i)
    {
        //        vf.emplace_back(async(launch::async, [&]{
        //            return accumulate(v.begin()+perThread*i, v.begin()+perThread*(i+1), 0); }));
        vf.emplace_back(async(launch::async, accumulate<std::vector<int>::const_iterator, int>,v.begin()+perThread*i,v.begin()+perThread*(i+1),0));
    }
    int sum = accumulate(v.begin()+perThread*threadNum,v.end(),0);
    for(future<int>& res : vf)
    {
        sum += res.get();
    }

    return sum;
}

void Task3()
{
    chrono::high_resolution_clock::time_point start, stop;
    int sum;
    vector<int> v(10000000, 1);
    //   for(unsigned long i=0;i<v.size();i++) v[i]=static_cast<int>(i);

    ///Use for_each
    start = chrono::high_resolution_clock::now();
    sum = accumulate(v.begin(), v.end(), 0);
    stop = chrono::high_resolution_clock::now();
    cout << sum << " Sequential time: ";
    showTime3(start,stop);

    start = chrono::high_resolution_clock::now();
    sum = parallel_sum(v, thread::hardware_concurrency()-1);
    stop = chrono::high_resolution_clock::now();
    cout << sum << " parallel_sum (thread=cores-1) time: ";
    showTime3(start,stop);

    start = chrono::high_resolution_clock::now();
    sum = parallel_sum(v, 2);
    stop = chrono::high_resolution_clock::now();
    cout << sum << " parallel_sum (2 threads) time: ";
    showTime3(start,stop);

    start = chrono::high_resolution_clock::now();
    sum = parallel_sum(v, 12);
    stop = chrono::high_resolution_clock::now();
    cout << sum << " parallel_sum (12 threads) time: ";
    showTime3(start,stop);
}
