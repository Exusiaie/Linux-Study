/**
 * Project Untitled
 */


#ifndef _PRODUCERSTHREAD_H
#define _PRODUCERSTHREAD_H

#include "Thread.h"
#include "Thread.h"


class ProducersThread: public Thread, public Thread {
public: 
    TaskQueue& _taskQue;
private: 
    
void run();
};

#endif //_PRODUCERSTHREAD_H