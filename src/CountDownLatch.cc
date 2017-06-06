#include "CountDownLatch.h"

using namespace jmuduo;


void CountDownLatch::wait()
{
	MutexLockGuard mutex(mutex_);
	while(count_ > 0)
	{
		cond_.wait();
	}

}

void CountDownLatch::countdown()
{
	MutexLockGuard mutex(mutex_);
	--count_;
	if(count_ == 0)
	{
		cond_.notifyAll();
	}
}

int CountDownLatch::getCount() const
{
	MutexLockGuard mutex(mutex_);
	return count_;
}