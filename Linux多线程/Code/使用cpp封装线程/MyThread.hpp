#ifndef __MyThread_H__
#define __MyThread_H__

#include "NonCopyable.hpp"
#include <pthread.h>

namespace Ning{

class MyThread : NonCopyable{
public:
    MyThread(); // 构造函数
   virtual  ~MyThread();            // 析构函数设为虚函数

    void start(); // 启动子线程
    void join(); // 等待子线程运行结束

private:
    virtual void run() = 0;
    static void* thread_function(void*); // 子线程入口函数, 参数是void*, 返回值也是void*, 代表着可以传递任何值

private:
    pthread_t _pid; // 线程的id
    bool _isRunning; // 线程的运行状态
};

} // end of namespace Ning



#endif


