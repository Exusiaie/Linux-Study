/**
 * Project Untitled
 */


#ifndef _CONSUMERSTHREAD_H
#define _CONSUMERSTHREAD_H

#include "Thread.h"


class ConsumersThread: public Thread {
public: 
    TaskQueue& _taskQue;
private: 
    
void run();
};

#endif //_CONSUMERSTHREAD_H