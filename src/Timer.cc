#include "Timer.h"

using namespace jmuduo;
using namespace jmuduo::net;

AtomicInt64 Timer::s_numCreated_;


Timer::Timer(const TimerCallback& callback,Timestamp when,double interval):
	callback_(callback),
	expiration_(when),
	repeated_(interval > 0),
	interval_(interval),
	sequence_(s_numCreated_.incrementAndGet())
{

}

void Timer::restart(Timestamp now)
{
	if(repeated_)
	{
		expiration_ = addTime(now,interval_);
	}
}