#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"
#include "TimerQueue.h"

#include <stdint.h>
#include <assert.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <boost/bind.hpp>
#include <signal.h>

using namespace jmuduo;
using namespace jmuduo::net;

namespace 
{
	__thread EventLoop* t_loopInThisThread = 0;
	const int KPollTimeMs = 10000;

	int createEventfd()
	{
		int ret = ::eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK);
		(void)ret;
		return ret;
	}

	class IgnoreSigPipe
	{
		public:
			IgnoreSigPipe()
			{
				::signal(SIGPIPE,SIG_IGN);
			}

	};

	IgnoreSigPipe initobj;
}

EventLoop::EventLoop():
	looping_(false),
	quit_(false),
	threadId_(CurrentThread::tid()),
	poller_(new Poller(this)),
	timerQueue_(new TimerQueue(this)),
	callingPendingFunctor_(false),
	wakeupFd_(createEventfd()),
	wakeChannel_(new Channel(this,wakeupFd_))
{
	if(t_loopInThisThread)
	{
		printf("EventLoop() error:Another EventLoop is in this Thread\n");
	}
	else
	{
		t_loopInThisThread = this;
	}
	wakeChannel_->setReadCallback(boost::bind(&EventLoop::handleRead,this,_1));
	wakeChannel_->enableReading();
}

EventLoop::~EventLoop()
{
	abortNotInLoopThread();
	assert(!looping_);
	t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
	assert(!looping_);
	abortNotInLoopThread();
	looping_ = true;
	quit_ = false;

	while(!quit_)
	{
		activeChannels_.clear();
		Timestamp now = poller_->poll(KPollTimeMs,&activeChannels_);
		for(ChannelList::iterator it = activeChannels_.begin();it!=activeChannels_.end();++it)
		{
			(*it)->handleEvent(now);
		}
		doPendingFunctors_();
	}
	
	printf("EventLoop stop looping\n");
	looping_ = false;
}
void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	abortNotInLoopThread();
	poller_->removeChannel(channel);
}
void EventLoop::quit()
{
	quit_ = true;
	if(!isInLoopThread())
	{
		wakeup();
	}
	//wakeup
}
void EventLoop::runInLoop(const Functor& cb)
{
	if(isInLoopThread())
	{
		cb();
	}
	else
	{
		queueInLoop(cb);
	}
}
void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	abortNotInLoopThread();
	poller_->updateChannel(channel);
}


TimerId EventLoop::runAt(const Timestamp& time,const TimerCallback& cb)
{
	return timerQueue_->addTimer(cb,time,0.0);
}
TimerId EventLoop::runAfter(double delay,const TimerCallback& cb)
{
	Timestamp timestamp(addTime(Timestamp::now(),delay));
	return runAt(timestamp,cb);
}
TimerId EventLoop::runEvery(double interval,const TimerCallback& cb)
{
	Timestamp timestamp(addTime(Timestamp::now(),interval));
	return timerQueue_->addTimer(cb,timestamp,interval);
}


void EventLoop::queueInLoop(const Functor& cb)
{
	{
		MutexLockGuard lock(mutex_);
		pendingFunctors_.push_back(cb);
	}

	if(!isInLoopThread() || callingPendingFunctor_)
	{
		wakeup();
	}
}


void EventLoop::handleRead(Timestamp receiveTime)
{
	uint64_t res = 1;
	::read(wakeupFd_,&res,sizeof(res));
	//check return value == 8 bytes
}

void EventLoop::wakeup()
{
	uint64_t res = 1;
	::write(wakeupFd_,&res,sizeof(res));
	//check return value == 8 bytes
}

void EventLoop::doPendingFunctors_()
{
	std::vector<Functor> functors;
	callingPendingFunctor_ = true;
	{
		MutexLockGuard lock(mutex_);
		functors.swap(pendingFunctors_);
	}
	for(int i=0;i<functors.size();++i)
	{
		functors[i]();
	}
	callingPendingFunctor_ = false;
}