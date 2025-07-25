#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H

#include "NonCopyabel.h"
#include <pthread.h>

namespace Ning{

class MutexLock : NonCopyable{
public:
    MutexLock();
    ~MutexLock();

    void  lock();
    void unlock();

    pthread_mutex_t* getMutexLockPtr(){ // 取得锁指针
        return &_mutex;
    }

private:
    pthread_mutex_t _mutex;
};



// 使用RAII思想, 将互斥锁的上锁和解锁托管
class MutexLockGuard{
public:
    MutexLockGuard(MutexLock & m)
        :_mutex(m){
            _mutex.lock(); // 调用上面那个类里的lock函数
        }
    ~MutexLockGuard(){
        _mutex.unlock();
    }

private:
    MutexLock & _mutex;
};


}



#endif //_MUTEXLOCK_H
