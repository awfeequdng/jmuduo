#ifndef __CONDITION_H
#define __CONDITION_H

#include <pthread.h>
#include "Mutex.h"

namespace jmuduo{

class Condition
{
	public:
		explicit Condition(MutexLock& mutex):
			mutex_(mutex)
		{
			MCHECK(pthread_cond_init(&cond_,NULL));
		}

		~Condition()
		{
			MCHECK(pthread_cond_destroy(&cond_));
		}

		void wait()
		{
			MutexLock::UnassignGuard unassignGuard(mutex_);
			MCHECK(pthread_cond_wait(&cond_,mutex_.getMutexLockPointer()));
		}

		void notify()
		{
			MCHECK(pthread_cond_signal(&cond_));
		}

		void notifyAll()
		{
			MCHECK(pthread_cond_broadcast(&cond_));
		}

	private:
		MutexLock& mutex_;
		pthread_cond_t cond_;
};




}


#endif