#include "task6.h"
#include "singleton.h"
#include "singletonthread.h"
#include <iostream>
#include <thread>

using namespace std;
void Task6Thread()
{
//    cout << SingletonThread::getInstance().num << endl;
    cout << SingletonThread::getInstance().num;
}

void Task6Test1()
{
    cout << Singleton::getInstance().num;
    cout << Singleton::getInstance().num;
    cout << endl;
}

void Task6Test2()
{
    thread t1(Task6Thread);
    thread t2(Task6Thread);
    Task6Thread();
    t1.join();
    t2.join();
    cout << endl;
}

void Task6()
{
    Task6Test1();
    Task6Test2();
}
