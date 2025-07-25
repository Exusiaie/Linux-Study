#include "Thread.h"

#include <stdio.h> // for strerror
#include <string.h>


namespace Ning{

// 构造函数
Thread::Thread()
    :_pthid(0),      // 初始化为0, 等待赋值
    _isRuning(false) 
{

}

// start函数
// 只有当一个线程没有在运行的时候, 才将它创建并运行
// pthread_create函数第四个参数是传递给线程入口函数的参数--
// 将this指针作为参数, 表示将这个子线程对象传入了线程入口函数
void Thread::start(){
    if(!_isRuning){
        int ret = pthread_create(&_pthid, nullptr, start_routine, this);
        if(ret != 0){
            fprintf(stderr, "%s", strerror(ret));
            return;
        }
        _isRuning = true;
    }
}

// join函数
// 让主线程等待子线程
// 只有当一个线程正在运行的时候, 才能让它join
// join函数结束时代表着子线程已经运行完毕了, 所以要设置标志位
// join的函数一定要能运行
void Thread::join(){
    if(_isRuning){
        pthread_join(_pthid, nullptr);
        _isRuning = false;
    }
}

// 这是一个静态的成员函数, 没有this指针
// 所以调用这个成员函数的pthread_create()(第23行) 只能用this指针的方法把Thread对象传递给它
// 又由于传递进来的参数是void*类型, 要进行类型转化
// 在线程入口函数内调用run函数, run代表的是具体要执行的任务
void* Thread::start_routine(void *arg){
    Thread* pthread = static_cast<Thread*>(arg);
    if(pthread != nullptr){
        pthread->run();
    }
    return nullptr;
}

}

