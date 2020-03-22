#include "queue.h"


Queue::Queue()
{
    pr = buf;
    pw = buf;
}

Queue::~Queue()
{
    unique_lock<mutex> lk(m);
    notify_all_at_thread_exit(wv, move(lk));
    notify_all_at_thread_exit(rv, move(lk));
}

Queue::Queue(const Queue &other)
{
    lock_guard<mutex> lock(other.m);
    for(unsigned long i = 0; i < sizeof(buf); i++)
    {
        buf[i] = other.buf[i];
    }
    pr = other.pr;
    pw = other.pw;
    elementsCount = other.elementsCount;
}

Queue &Queue::operator=(const Queue &other)
{
    if(&other == this ) return *this;
        unique_lock<mutex> lock1(m, defer_lock);
        unique_lock<mutex> lock2(other.m, defer_lock);
        lock(lock1, lock2);
        for(unsigned long i = 0; i < sizeof(buf); i++)
        {
            buf[i] = other.buf[i];
        }
        pr = other.pr;
        pw = other.pw;
        elementsCount = other.elementsCount;
        return *this;
}

void Queue::pop(char &d)
{
    unique_lock<mutex> locker(m);
    rv.wait(locker,[&](){return elementsCount;});
    d = *pr;
    ++pr;
    if(pr-buf == queueSize) pr = buf;
    --elementsCount;
    wv.notify_one();
}

void Queue::push(char d)
{
    unique_lock<mutex> locker(m);
    wv.wait(locker, [&](){return elementsCount < queueSize;});
    *pw = d;
    ++pw;
    if(pw-buf == queueSize) pw = buf;
    ++elementsCount;
    rv.notify_one();
}
