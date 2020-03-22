#ifndef SINGLETON_H
#define SINGLETON_H


class Singleton
{
private:

    Singleton() {++num;}
    Singleton( const Singleton&);
    Singleton& operator=( Singleton& );
public:
    static unsigned int num;
    static Singleton& getInstance()
    {
        static Singleton instance;
        return instance;
    }
};


#endif // SINGLETON_H
