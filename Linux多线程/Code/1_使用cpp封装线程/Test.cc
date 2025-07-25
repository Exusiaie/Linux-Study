#include "MyThread.hpp"
#include <iostream>

#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class Run 
:public Ning::MyThread{
    void run() override{
        std::cout << "执行了run方法" << std::endl;
    }
};

int main(){
    unique_ptr<Ning::MyThread> thread(new Run);

    thread->start();
    thread->join();
    std::cout << "主线程即将结束" << endl;
    return 0;
}


