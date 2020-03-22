#ifndef SINGLETONTHREAD_H
#define SINGLETONTHREAD_H


class SingletonThread
{
private:
    SingletonThread() {++num;}
    SingletonThread( const SingletonThread&);
    SingletonThread& operator=( SingletonThread& );
public:
    static unsigned int num;
    static SingletonThread& getInstance()
    {
        thread_local SingletonThread instance;
        return instance;
    }
};

#endif // SINGLETONTHREAD_H
