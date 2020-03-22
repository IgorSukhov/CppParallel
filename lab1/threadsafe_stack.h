#ifndef THREADSAFE_STACK_H
#define THREADSAFE_STACK_H

#include <vector>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <iomanip>
#include <iostream>

using namespace std;

template <typename T>
class threadsafe_stack
{
    vector<int> v;
    mutable shared_mutex m;

public:

    threadsafe_stack() = default;
    ~threadsafe_stack() = default;
    explicit threadsafe_stack(const threadsafe_stack<T> &other);
    threadsafe_stack(threadsafe_stack<T> &&other);
    threadsafe_stack<T> &operator= (const threadsafe_stack<T> &other);
    threadsafe_stack<T> &operator= (threadsafe_stack<T> &&other);

    vector<int> &getVector() { return v; }
    const mutex *getMutex() const { return &m; }

    void push(T val);
    void pop(T& val);
    std::shared_ptr<T> pop();

    void showStack() const;
private:

};

template<typename T>
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack<T> &other)
{
    lock_guard lock(other.m);
    unsigned long len = other.v.size();
    v.reserve(len);
    for(unsigned long i =0; i < len; i++) v.push_back(other.v[i]);
}

template<typename T>
threadsafe_stack<T>::threadsafe_stack(threadsafe_stack<T> &&other)
{
    scoped_lock s(m, other.m);
    v=move(other.v);
}

template<typename T>
threadsafe_stack<T> &threadsafe_stack<T>::operator=(const threadsafe_stack<T> &other)
{
    if(&other == this ) return *this;
    unique_lock<shared_mutex> lock1(m, defer_lock);
    shared_lock<shared_mutex> lock2(other.m, defer_lock);
    lock(lock1, lock2);
    v=other.v;
    return *this;
}

template<typename T>
threadsafe_stack<T> &threadsafe_stack<T>::operator=(threadsafe_stack<T> &&other)
{
    scoped_lock s(m, other.m);
    v=move(other.v);
    return *this;
}

template<typename T>
void threadsafe_stack<T>::push(T val)
{
    std::lock_guard<shared_mutex> lock(m);
    v.push_back(val);
}

template<typename T>
void threadsafe_stack<T>::pop(T &val)
{
    std::lock_guard<shared_mutex> lock(m);
    if(v.empty()) throw;
    val = std::move_if_noexcept(v.back());
    v.pop_back();
}

template<typename T>
shared_ptr<T> threadsafe_stack<T>::pop()
{
    std::lock_guard<shared_mutex> lock(m);
    if(v.empty()) throw;
    T *val = new T(move_if_noexcept(v.back()));
  //  *val = v.back();
    std::shared_ptr<T> p(val);
    v.pop_back();
    return p;
}

template<typename T>
void threadsafe_stack<T>::showStack() const
{
    for(const auto& i:v)
    {
        cout << i << " ";
    }
    std::cout << endl;
}

#endif // THREADSAFE_STACK_H
