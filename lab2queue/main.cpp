#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <atomic>

using namespace std;

class MyQueue {
public:
    mutable std::mutex m_mutex;
    std::condition_variable cv_for_push;
    std::condition_variable cv_for_pop;
    static const size_t size = 50;
    char buf[size]{};
    std::atomic<bool> stop_flag{ false }; //задействуем в дальнейшем
    size_t i_Push{}; //индекс первого свободного
    size_t i_Pop{}; //индекс первого занятого
    size_t m_currentSize{}; //текущее количество заявок в очереди

    std::atomic <size_t> m_readers{};// количество ожидающих на условной переменной читателей
    std::atomic <size_t> m_writers{};//количество ожидающих на условной переменной писателей

    MyQueue() = default;
    ~MyQueue()
    {
        stop_flag = true;
        while(m_readers && m_writers)
        {
            this_thread::sleep_for(100ms);
        }
    }
    //Читателей и писателей может быть разное количество!
    //Если писателей больше, то читатели (которые ждут появления заявки, но не дождутся) не должны "зависнуть"!
    MyQueue(const MyQueue&) = delete; //иначе непонятно, что делать с заявками
    MyQueue& operator= (const MyQueue&) = delete; //аналогично
    MyQueue(MyQueue&& other)noexcept
    {
        std::unique_lock lk(m_mutex);
        for(size_t i=0;i<size;i++)
        {
            buf[i]= other.buf[i];
        }
        if(other.stop_flag) stop_flag = true; else stop_flag = false;
        i_Push = other.i_Push;
        i_Pop = other.i_Pop;
        m_currentSize = other.m_currentSize;
    }
    //а перемещение можно разрешить, например,
    //для того чтобы передавать в функцию или возвращать по значению
    MyQueue& operator= (MyQueue&& other) noexcept
    {
        scoped_lock(m_mutex,other.m_mutex);
        for(size_t i=0;i<size;i++)
        {
            buf[i]= other.buf[i];
        }
        if(other.stop_flag) stop_flag = true; else stop_flag = false;
        i_Push = other.i_Push;
        i_Pop = other.i_Pop;
        m_currentSize = other.m_currentSize;
        return *this;
    }//а здесь может получится еще интереснее

    void realPop(char& c)
    {
        c = buf[i_Pop];
        i_Pop = (i_Pop + 1) % size;
        --m_currentSize;
    }

    void realPush(char& c)
    {
        buf[i_Push] = c;
        i_Push = (i_Push + 1) % size;
        ++m_currentSize;
    }

    void push(char c)
    {
        std::unique_lock lk(m_mutex);
        //Если в очереди есть место, сразу под защитой добавляем нашу заявку и завершаем функцию.
        //если места нет, ждем уведомления (или если ждать безнадежно, просто завершаемся)
        if (m_currentSize == size) //места нет
        {
            ++m_writers; //текущее количество ожидающих писателей
            //Если места нет. Ждем заданный timeout.
            //Если пришло уведомление. Уведомление могло придти, если
            //а) освободилось место в очереди => занимаем его и завершаем функцию
            //б) если работа потока была остановлена (кто-то установил признак)
            //просто выходим из функции
            //Если истек timeout, а уведомление так и не пришло, завершаем поток
            using namespace std::chrono_literals;
            if (cv_for_push.wait_for(lk, 1s, [this]() {return (stop_flag || m_currentSize < size);}) == false)
            {
                return;
            } //завершаем поток, так как timeout истек, место в буфере было, а читатель
            //так и не появился...
        }
        //В очереди или было место или пришло уведомление
        //Если пришло уведомление, но установлен stop_flag,
        //то нужно завершиться
        if (stop_flag)   {  return; }
        realPush(c); //под защитой
        std::cout << static_cast<char>(toupper(c)); //для отладки - писатели будут выводить большую букву, а читатели маленькую
        //Если есть ожидающий читатель, то уведомляем
        if (m_readers)
        {
            --m_readers;
            cv_for_pop.notify_one();
        }
    }
    //Разбираемся с pop:

    void pop() //для упрощения задачи пусть pop() имитирует обработку заявки - просто распечатывает значение и изымает его из очереди
    {
        using namespace std::chrono_literals;
        std::unique_lock lk(m_mutex);
        if (m_currentSize == 0) //заявок в очереди на текущий момент нет
        {
            ++m_readers;
            if (cv_for_pop.wait_for(lk, 1s, [this]()->bool {return /*stop_flag ||*/ m_currentSize;}) == false)
            {
                return; //timeout закончился, а уведомление так и не пришло
            }
        }
        //Пришло уведомление
        // if (stop_flag)  {  return;    }если пришло уведомление, но установлен флаг завершения
        char c;
        realPop(c);

        if(m_writers) //если есть ожидающие писатели
        {
            --m_writers;
            cv_for_push.notify_one();
        }
        std::cout << static_cast<char>(tolower(c)); //для отладки

    }

    size_t actual_size()const
    {
        std::unique_lock lk(m_mutex);
        return m_currentSize;
    }

    bool empty()const
    {
        std::unique_lock lk(m_mutex);
        return m_currentSize;
    }

    void stopWork()
    {
        stop_flag = true;
    }
};

int main()
{
    std::vector<std::thread>  v;
    v.reserve(40);
    //{
    MyQueue q;
    for (int i = 0; i < 10; i++)
    {
        v.push_back(std::thread(&MyQueue::push, &q, static_cast<char>('A' + i)));
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    for (int i = 0; i < 10; i++)
    {
        v.push_back(std::thread(&MyQueue::pop, &q));
    }

    for (char c = 'A' + 10; c < ('A' + 20); c++)
    {
        v.push_back(std::thread(&MyQueue::push, &q, c));
    }

    for (int i = 0; i < 10; i++)
    {
        v.push_back(std::thread(&MyQueue::pop, &q));
    }
    q.stopWork();

    for (auto& t : v)
    {
        t.join();
    }
    __asm("nop");
}
