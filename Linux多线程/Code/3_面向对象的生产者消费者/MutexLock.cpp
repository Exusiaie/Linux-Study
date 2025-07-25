#include "MutexLock.h"

namespace Ning{

// 锁的初始化只能使用初始化函数, 给它赋值nullptr是无用的
MutexLock::MutexLock()
{
    pthread_mutex_init(&_mutex, NULL);
}
// 同样的锁的删除也需要专门的函数
MutexLock::~MutexLock(){
    pthread_mutex_destroy(&_mutex);
}

void MutexLock::lock(){
    pthread_mutex_lock(&_mutex);
}

void MutexLock::unlock(){
    pthread_mutex_unlock(&_mutex);
}



}
