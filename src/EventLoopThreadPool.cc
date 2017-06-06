#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"

#include <assert.h>

using namespace jmuduo;
using namespace jmuduo::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop):
	baseloop_(loop),
	started_(false),
	nextId_(0),
	numThreads_(0)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{

}


void EventLoopThreadPool::start()
{
	assert(!started_);
	baseloop_->abortNotInLoopThread();
	started_ = true;
	if(numThreads_ > 0)
	{
		for(int i=0;i<numThreads_;++i)
		{
			EventLoopThread* eventLoopThread = new EventLoopThread();
			threads_.push_back(eventLoopThread);
			loops_.push_back(eventLoopThread->startLoop());
		}
	}
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
	EventLoop* loop = baseloop_;
	if(!loops_.empty())
	{
		loop = loops_[nextId_];
		++nextId_;
		if(nextId_==loops_.size())
		{
			nextId_ = 0;
		}
	}

	return loop;
}