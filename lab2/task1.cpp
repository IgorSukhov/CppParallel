#include "task1.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
struct Sum
{
    Sum(): sum{0} { }
    void operator()(int n) { sum += n; }
    int sum;
};

void showTime(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point stop)
{
    chrono::duration<double> time_elapsed = stop - start;
    cout << chrono::duration_cast<chrono::microseconds>(time_elapsed).count() << " us" << endl;
}
void transformer(std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
    auto iter = begin;
    while(iter != end)
    {
        //*iter=abs(*iter);
        ++(*iter);
        iter++;
    }
}

void transformVector(vector<int> &v, unsigned int threadCount)
{
    if(!threadCount) return;
    unsigned long elementsPerThread = v.size()/threadCount;
    vector<thread> vt;
    vt.reserve(threadCount);//static_cast<unsigned int>(
    for(unsigned int i = 0; i < threadCount-1; i++)
    {
        vt.emplace_back(transformer,v.begin()+static_cast<int>(elementsPerThread*i),v.begin()+static_cast<int>(elementsPerThread*(i+1)));
    }
    transformer(v.begin()+static_cast<int>(elementsPerThread*(threadCount-1)),v.end());
    for(auto& vt_el : vt)
    {
        vt_el.join();
    }
}

void Task1()
{
    chrono::high_resolution_clock::time_point start, stop;

    unsigned long long vecSize = 10000000;
    vector<int> v(vecSize);
    for_each(v.begin(),v.end(),[](int &n){n=1;});

    ///Use for_each
    start = chrono::high_resolution_clock::now();
    for_each(v.begin(),v.end(),[](int &n){++n;});
    stop = chrono::high_resolution_clock::now();
    cout << "for_each time: ";
    showTime(start,stop);

    ///Use transform
    start = chrono::high_resolution_clock::now();
    transform(v.begin(),v.end(),v.begin(),[](int &n)->int{return ++n;});
    stop = chrono::high_resolution_clock::now();
    cout << "transform time: ";
    showTime(start,stop);

    ///Use parallel transform
    start = chrono::high_resolution_clock::now();
    transformVector(v,thread::hardware_concurrency()-1);
    stop = chrono::high_resolution_clock::now();
    cout << "parallel transform time: ";
    showTime(start,stop);

    for_each(v.begin(),v.begin()+10,[](int &n){ cout << n << " "; });
    cout << endl;
}
