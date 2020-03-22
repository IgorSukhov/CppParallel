#include "task4.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <cstdlib>

using namespace std;

#define MIN(A,B) (A)<(B)?(A):(B)

void transformer(std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
    auto iter = begin;
    while(iter != end)
    {
        *iter=abs(*iter);
        iter++;
    }
}

void transformVector(vector<int> &v, unsigned int threadCount)
{
    if(!threadCount) return;
    unsigned int elementsPerThread = v.size()/threadCount;
    std::vector<thread> vt;
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

void showTimeTask4(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point stop)
{
    chrono::duration<double> time_elapsed = stop - start;
    cout << chrono::duration_cast<chrono::milliseconds>(time_elapsed).count() << " ms" << endl;
}

void Task4()
{
    unsigned int coresCount = thread::hardware_concurrency();
    cout << "Cores count: " << coresCount << endl;

    //    cout << "Choose cores count:";
    //    unsigned int coresCountChoosed;
    //    cin >> coresCountChoosed;
    //    coresCountChoosed = MIN(coresCountChoosed,coresCount);

    unsigned int testDataSize = 100000000;
    vector<int> v;
    v.reserve(testDataSize);
    for(int i=0;i<static_cast<int>(testDataSize);i++) v.push_back(i-static_cast<int>(testDataSize));

    chrono::high_resolution_clock::time_point start, stop;

    for(unsigned int threadsUsed = 1; threadsUsed <8; threadsUsed++)
    {
        start = chrono::high_resolution_clock::now();
        transformVector(v,threadsUsed);
        stop = chrono::high_resolution_clock::now();
        cout << "Threads used: " << threadsUsed << " time elapsed: ";
        showTimeTask4(start,stop);
    }
}
