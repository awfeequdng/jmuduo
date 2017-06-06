#ifndef __EVENTLOOPTHREAD_H
#define __EVENTLOOPTHREAD_H

#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

namespace jmuduo
{

namespace net
{


class EventLoop;

class EventLoopThread
{
	public:
		EventLoopThread();
		
		~EventLoopThread();

		EventLoop* startLoop();

		void threadFunc();



	private:
		EventLoop* loop_;
		Thread thread_;
		MutexLock mutex_;
		Condition cond_;

};


}

}


#endif