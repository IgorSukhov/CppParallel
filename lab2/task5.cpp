#include "task5.h"
#include "queue.h"
#include <iostream>
#include <thread>

void pusher(Queue &q)
{
    for(char ch = 'A'; ch <= 'Z'; ch++)
    {
        q.push(ch);
        cout << "-> " << ch << endl;
    }
}

void poper(Queue &q)
{
    char chpop;
    for(char ch = 'A'; ch <= 'Z'; ch++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        q.pop(chpop);
//        cout << "<- " << chpop << endl;
    }
}

void Test1()
{
    Queue q;
    thread thPush(pusher,ref(q));
    thread thPop(poper,ref(q));
    this_thread::sleep_for(chrono::milliseconds(250));

    thPush.join();
    thPop.join();
}

void Test2()
{
    Queue q1,q3;
    q1.push('A');
    q1.push('B');
    Queue q2(q1);
    char ch;
    q2.pop(ch);
    cout << ch << endl;
    q2.pop(ch);
    cout << ch << endl;
    q3 = q1;
    q3.pop(ch);
    cout << ch << endl;
    q3.pop(ch);
    cout << ch << endl;
    cout << endl;
}

void Task5()
{
//    Test1();
    Test2();
}
