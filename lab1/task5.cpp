#include "task5.h"
#include "threadsafe_stack.h"

#include <thread>
#include <iostream>
using namespace std;

void pusher(threadsafe_stack<int> &st, int add)
{
    for(int i=0;i<10;i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        st.push(i+add);
    }
}

void poper(threadsafe_stack<int> &st)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    int val;
    for(int i=0;i<10;i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        st.pop(val);
        cout << val << " ";
    }
}

void Test2()
{
    threadsafe_stack<int> st1, st2;
    st1.push(3);
    st1.push(5);
    st1.push(7);
    st2 = st1;
    st2.pop();
    threadsafe_stack<int> st3(st1);
    //threadsafe_stack<int> st4 = st1;
    threadsafe_stack<int> st5 = threadsafe_stack<int>(st1);
    vector<threadsafe_stack<int>> v;
    v.reserve(5);
    v.push_back(st1);
    v.push_back(st2);
    v.push_back(st3);
    //v.push_back(st4);
    v.push_back(st5);
    for(auto& el : v)
    {
        el.showStack();
    }
}

void Test1()
{
    threadsafe_stack<int> st;
    try
    {
        //st.pop();
        throw 100;
    }
    catch(int)
    {
        cout << "pop from empty stack" << endl;
    }
    thread th1(pusher,std::ref(st),10);
    thread th2(pusher,std::ref(st),20);
    thread th3(pusher,std::ref(st),30);
    thread thReader(poper,std::ref(st));

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    threadsafe_stack<int> st2(st);
    th1.join();th2.join();th3.join();
    thReader.join();

    std::cout << endl;
    st.showStack();
    st2.showStack();
}

void Task5()
{
    //Test1();
    Test2();
}
