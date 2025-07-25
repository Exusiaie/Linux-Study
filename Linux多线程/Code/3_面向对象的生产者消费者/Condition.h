#ifndef _CONDITION_H
#define _CONDITION_H

#include "NonCopyable.h"
#include <pthread.h>

namespace Ning{

class MutexLock; // 前向声明, 防止出现头文件的循环依赖(多个头文件直接或间接的互相包含)


class Condition : NonCopyable{
public:
    Condition(MutexLock &);
    ~Condition();

    void wait();
    void notifOne();
    void notifAll();

private:
    pthread_cond_t _cond;
    MutexLock & _mutex;
};


}
#endif //_CONDITION_H
