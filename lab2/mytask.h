#ifndef MYTASK_H
#define MYTASK_H

#include <vector>
#include <numeric>

using namespace  std;

class MyTask
{
public:
    MyTask();
    void operator()();
    void taskToDo();
    int result;
};

#endif // MYTASK_H
