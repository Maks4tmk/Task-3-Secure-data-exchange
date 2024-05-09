// Защищённый обмен данными.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <mutex>
#include <thread>
#include <Windows.h>

class Data {
private:
    int data_;
public:
    Data(int data) :data_(data) {};
    int get() { return data_; }
    void set(int data) { data_ = data; }
    std::mutex mutex_;

    void swap_Lock(Data& d2)
    {
        std::lock(this -> mutex_, d2.mutex_);

        int tmp = this -> get();
        this -> set(d2.get());
        d2.set(tmp);
        std::cout << "=========Lock=========" << std::endl;
        this->print();
        d2.print();
        this -> mutex_.unlock();
        d2.mutex_.unlock();
    }

    void swap_ScopedLock(Data& d2)
    {
        std::scoped_lock lock(this->mutex_, d2.mutex_);

        int tmp = this->get();
        this->set(d2.get());
        d2.set(tmp);
        std::cout << "=========ScopedLock=========" << std::endl;
        this->print();
        d2.print();
    }

    void swap_UniqueLock(Data& d2)
    {
        std::unique_lock<std::mutex> lock1(this->mutex_, std::defer_lock);
        std::unique_lock<std::mutex> lock2(d2.mutex_, std::defer_lock);
        lock1.lock();
        lock2.lock();

        int tmp = this->get();
        this->set(d2.get());
        d2.set(tmp);

        std::cout << "=========UniqueLock=========" << std::endl;
        this->print();
        d2.print();

        lock1.unlock();
        lock2.unlock();
    }

    void print() {
        std::cout << "data_ = " << data_ << ";" << std::endl;
    }
};


int main()
{
    Data d1(12);
    Data d2(124);

    d1.print();
    d2.print();


    std::thread t1([&]() {
        d1.swap_Lock(d2);
        });
    std::thread t2([&]() {
        d1.swap_ScopedLock(d2);
        });
    std::thread t3([&]() {
        d1.swap_UniqueLock(d2);
        });

    t1.join();
    t2.join();
    t3.join();
   
}
