#ifndef _THREAD_H__
#define _THREAD_H__

#include "NonCopyabel.h"
#include <pthread.h>

namespace Ning{

class Thread : NonCopyable{
public:
    Thread();
    virtual ~Thread() {}

    void start();   // 启动一个子线程
    void join();    // 等待一个子线程结束


private: 
    virtual void run() = 0;             // 虚函数run, 表示派生类要自己去实现, 代表子线程具体要做的事
    static void* start_routine(void*);  // 子线程入口函数   

private:
    pthread_t _pthid;   // 线程id
    bool _isRuning;     // 用于防止虚假唤醒的标志位                
};

}



#endif //_THREAD_H
