/**
 * Project Untitled
 */


#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H

class TaskQueue {
public: 
    queue<int> _que;
    int _queSize;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    
/**
 * @param int
 */
void TaskQueue(void int);
    
bool empty();
    
bool full();
    
/**
 * @param int
 */
void push(void int);
    
int pop();
};

#endif //_TASKQUEUE_H