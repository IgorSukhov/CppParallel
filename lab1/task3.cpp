#include "task3.h"

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

void Task3()
{
    chrono::milliseconds t = chrono::milliseconds(10);
    chrono::milliseconds dt = chrono::milliseconds(10);

    for(char i='A';i<='Z';i++)
    {
        cout << i;
        this_thread::sleep_for(t);
        t+=dt;
    }
    cout << endl;
}
