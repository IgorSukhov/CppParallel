#include "task2.h"
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>


using namespace std;

vector<thread::id> A::messages;
mutex A::m;
map<thread::id,string> A::id_name;
unsigned int A::thNum = 0;

A::A()
{
    lock_guard<mutex> g(m);
    messages.emplace_back(this_thread::get_id());
}

void ThreadFunc(string name)
{
    A a1;
    A* pA = new A();
    thread_local A a2;
    static A a3;
    A::id_name.insert(make_pair(this_thread::get_id(),name));
    delete pA;
}

void Task2()
{
    unsigned long threadsNum = 7;

    vector<thread> vTh;
    vTh.reserve(threadsNum);

    vector<thread::id> ids;
    ids.reserve(threadsNum);

    cout << "Launched thread count : " << threadsNum << endl;

    for(unsigned long i = 0; i < threadsNum; i++)
    {
        vTh.emplace_back(ThreadFunc,"th#" + to_string(i+1));//
        ids.push_back(vTh[i].get_id());
    }

    for(thread& t : vTh)
    {
        t.join();
    }

    cout << "Count of created objects: " << A::messages.size() << endl;

    for(unsigned long i = 0; i < threadsNum; i++)
    {
        cout << "Name: " << A::id_name.find(ids[i])->second
             << " Launch order: " << i+1
             << " Objects created: " << count(A::messages.begin(),A::messages.end(),ids[i]);
        cout << endl;
    }
}
