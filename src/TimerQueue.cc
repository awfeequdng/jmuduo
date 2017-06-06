#include "TimerQueue.h"
#include "TimerId.h"
#include "Timer.h"
#include <sys/timerfd.h>
#include <boost/bind.hpp>
#include <stdint.h>
#include <assert.h>
#include <strings.h>
#include <algorithm> //std::copy
#include <iterator> //std::back_inserter

namespace jmuduo
{

namespace detail
{
struct timespec howmuchTimeFromNow(Timestamp timestamp)
{
	int64_t diff = timestamp.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
	if(diff < 100)
	{
		diff = 100;
	}

	struct timespec is;
	is.tv_sec = static_cast<time_t>(diff / Timestamp::kMicroPerSecond);
	is.tv_nsec = static_cast<long>((diff % Timestamp::kMicroPerSecond) * 1000);

	return is;
}
int createTimerfd()
{
	return ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
}
void readTimerfd(int timerfd,Timestamp now)
{
	uint64_t howmany;
	ssize_t n = ::read(timerfd,&howmany,sizeof(howmany));
	assert(n == sizeof(howmany));
	(void)now;
}
void resetTimerfd(int fd,Timestamp timestamp)
{
	struct itimerspec newvalue;
	struct itimerspec oldvalue;
	bzero(&newvalue,sizeof(newvalue));
	bzero(&oldvalue,sizeof(oldvalue));
	newvalue.it_value= howmuchTimeFromNow(timestamp);
	int ret = ::timerfd_settime(fd,0,&newvalue,&oldvalue);
	assert(!ret);
	(void)ret;
}


}

}





using namespace jmuduo;
using namespace jmuduo::net;


TimerQueue::TimerQueue(EventLoop* loop):
	loop_(loop),
	timerfd_(detail::createTimerfd()),
	timerfdChannel_(loop,timerfd_),
	timers_()
{
	timerfdChannel_.setReadCallback(boost::bind(&TimerQueue::handleRead,this,_1));
	timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
	timerfdChannel_.disableAll();
	timerfdChannel_.remove();
	::close(timerfd_);
	for(TimerList::iterator it=timers_.begin();it!=timers_.end();++it)
	{
		delete it->second;
	}
}



TimerId TimerQueue::addTimer(const TimerCallback& cb,Timestamp when,double interval)
{
	Timer* timer=new Timer(cb,when,interval);
	loop_->runInLoop(boost::bind(&TimerQueue::addTimerInLoop,this,timer));
	return TimerId(timer,timer->sequence());
}

void TimerQueue::addTimerInLoop(Timer * timer)
{
	loop_->abortNotInLoopThread();
	bool earliestChanged = insert(timer);
	if(earliestChanged)
	{
		detail::resetTimerfd(timerfd_,timer->expiration());
	}
}

bool TimerQueue::insert(Timer* timer)
{
	loop_->abortNotInLoopThread();
	bool earliestChanged = false;
	Timestamp when = timer->expiration();
	TimerList::iterator it = timers_.begin();
	if(it==timers_.end() | when < it->first )
	{
		earliestChanged = true;
	}
	std::pair<TimerList::iterator,bool> result =timers_.insert(Entry(when,timer));
	assert(result.second == true);
	(void)result;
	return earliestChanged;


}


void TimerQueue::handleRead(Timestamp receiveTime)
{
	
	Timestamp now(Timestamp::now());
	detail::readTimerfd(timerfd_,now);
	std::vector<Entry> expired = getExpired(now);
	for(std::vector<Entry>::iterator it = expired.begin();it!=expired.end();++it)
	{
		it->second->run();
	}

	reset(expired,now);
}


void TimerQueue::reset(std::vector<Entry>& expired,Timestamp now)
{
	Timestamp nextExpired;
	for(std::vector<Entry>::iterator it =expired.begin();it!=expired.end();++it)
	{	
		
		if(it->second->repeated())
		{
			it->second->restart(now);
			insert(it->second);
		}
		else
		{
			delete it->second;
		}
	}
	if(!timers_.empty())
	{
		nextExpired = timers_.begin()->first;
	}
	if(nextExpired.valid())
	{
		detail::resetTimerfd(timerfd_,nextExpired);
	}
}


std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
	std::vector<Entry> expired;
	Entry sentry = std::make_pair<Timestamp,Timer*>(now,reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator it = timers_.lower_bound(sentry);
	assert(it == timers_.end() | now < it->first);
	std::copy(timers_.begin(),it,back_inserter(expired));
	timers_.erase(timers_.begin(),it);
	return expired;

}