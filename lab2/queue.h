#ifndef QUEUE_H
#define QUEUE_H

#include <condition_variable>
#include <iostream>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <memory>


#include <vector>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <iomanip>
#include <iostream>
using namespace std;

class Queue
{
    const unsigned int queueSize = 10;
    unsigned int elementsCount = 0;
    mutable mutex m;
    char buf[10];
    char *pr;
    char *pw;
    condition_variable rv, wv;
public:
    Queue();
    ~Queue();
    explicit Queue(const Queue &other);
    Queue(Queue &&other) = delete;
    Queue &operator= (const Queue &other);
    Queue &operator= (Queue &&other) = delete;

    void push(char d);
    void pop(char &d);
};

#endif // QUEUE_H
