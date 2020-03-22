#ifndef TASK2_H
#define TASK2_H

#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <map>

using namespace std;

void Task2();

class A
{
    static mutex m;
    static unsigned int thNum;
public:
    A();
    static mutex mId;
    static vector<thread::id> messages;
    static map<thread::id, string> id_name;
};

#endif // TASK2_H
