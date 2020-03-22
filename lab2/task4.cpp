#include "task4.h"
#include <future>
#include <string>
#include <iostream>
#include <exception>
using namespace std;

int ConvertInput()
{
    string str;
    cin >> str;
    int val;
    try {
        val = stoi(str);
    } catch (invalid_argument &e) {
        cout << "other thread throwed!!!!!" << endl;
        throw e;
    }

    return val;
}

void Task4()
{
    future<int> f = async(launch::async,ConvertInput);

    try {
        cout << f.get() << " no exeption" << endl;
    } catch (invalid_argument e) {
        cout << "main thread throwed!!!!!" << endl;
    }
}
