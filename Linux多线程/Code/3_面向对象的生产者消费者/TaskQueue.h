#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

#include "MutexLock.h"
#include "Condition.h"
#include <queue>
using std::queue;

namespace Ning{

class TaskQueue {
public:
    TaskQueue(int);
    bool empty() const; // const表示这个函数不会修改任何值
    bool full() const;
    void push(int);
    int pop();

private: 
    queue<int>  _que;
    int         _queSize;
    MutexLock   _mutex;
    Condition   _notFull;
    Condition   _notEmpty;
};

}
#endif //_TASKQUEUE_H
