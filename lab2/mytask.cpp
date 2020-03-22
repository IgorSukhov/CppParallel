#include "mytask.h"

#include <iostream>
#include <vector>

MyTask::MyTask()
{

}

void MyTask::operator()()
{

}

void MyTask::taskToDo()
{
    vector<int> v(100000,1);
    result = accumulate(v.begin(),v.end(),0);
}
