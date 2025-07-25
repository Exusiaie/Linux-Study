#include "TaskQueue.h"
#include "MutexLock.h"
#include <iostream>

using std::cout;
using std::endl;

namespace Ning{

// 初始化任务队列
TaskQueue::TaskQueue(int queSize) 
{

}

/**
 * @return bool
 */
bool TaskQueue::empty() {
    return false;
}

/**
 * @return bool
 */
bool TaskQueue::full() {
    return false;
}

/**
 * @param int
 * @return void
 */
void TaskQueue::push(void int) {
    return;
}

/**
 * @return int
 */
int TaskQueue::pop() {
    return 0;
}

}
