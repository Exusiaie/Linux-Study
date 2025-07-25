#include "MyThread.hpp"

#include <iostream>

using std::cout;
using std::endl;

namespace Ning{

MyThread::MyThread()
    :_pid(0)    // 线程ID初始化为0
    ,_isRunning(false)  // 线程运行状态初始化false
{

}

MyThread::~MyThread(){

}

void MyThread::start(){
    if(!_isRunning){ // 只有线程还没开始运行时才能进下面的逻辑
        int ret = pthread_create(&_pid, nullptr, thread_function, this); // this指针将start这个对象传递给了线程入口函数
        if(ret != 0){
            cout << "错误" << endl;
            return;
        }          
        _isRunning = true; // 线程开始运行
    }
}

void MyThread::join(){
    if(_isRunning){
        pthread_join(_pid, nullptr);
        _isRunning = false; // 代码运行到这里表示join函数退出, 线程函数结束, 需要更改子线程状态
    }
}

// 为什么类中定义的函数是private但还是可以在这里访问?
// private 关键字控制了 访问权限（谁可以调用该函数），而 static 关键字决定了 所有权（函数属于类，而不是对象）并影响了它如何作为函数指针使用。
// 你可以将一个 static private 成员函数的实现定义在类外部，但它的使用仍然被限制在类内部（或通过友元）。
void* MyThread::thread_function(void *arg){ // 线程入口函数的参数接收了start传来的对象
    MyThread *pthread = static_cast<MyThread*>(arg); // 更安全的类型转化
    if(pthread){
        pthread->run(); // run()方法是线程具体要做的事情的接口
    }
    return nullptr; // 没有具体要做的事情就返回空指针
}


} // end of namespace Ning
