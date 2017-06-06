#ifndef __MUTEX_H
#define __MUTEX_H

#include <pthread.h>
#include <assert.h>

#include "CurrentThread.h"


#define MCHECK(ret) ({__typeof__(ret) errnum = (ret);\
					  assert(errnum==0);(void)errnum;})

namespace jmuduo{

class MutexLock
{
	public:
	MutexLock():
		holder_(0)
	{
		MCHECK(pthread_mutex_init(&mutex_,NULL));
	}
	~MutexLock()
	{
		pthread_mutex_destroy(&mutex_);
	}

	void lock()
	{
		
		MCHECK(pthread_mutex_lock(&mutex_));
		assignHolder();
	}

	void unlock()
	{
		
		unassignHolder();
		MCHECK(pthread_mutex_unlock(&mutex_));
	}

	void unassignHolder()
	{
		holder_=0;
	}

	void assignHolder()
	{
		holder_ = static_cast<pid_t>(CurrentThread::tid());
	}

	pthread_mutex_t* getMutexLockPointer()
	{
		return &mutex_;
	}
	private:
		pthread_mutex_t mutex_;
		pid_t holder_;
		friend class Condition;
		class UnassignGuard
		{
			public:
			UnassignGuard(MutexLock& owner):owner_(owner)
			{
				owner_.unassignHolder();
			}

			~UnassignGuard()
			{
				owner_.assignHolder();
			}
			private:
				MutexLock& owner_;
		};
};

class MutexLockGuard
{
	public:
		MutexLockGuard(MutexLock& mutex):
			mutex_(mutex)
		{
			mutex_.lock();
		}
		~MutexLockGuard()
		{
			mutex_.unlock();
		}
	private:
		MutexLock& mutex_;
};

#define MutexLockGuard(x) error"Missing MutexLockGuard Object Name"
}



#endif