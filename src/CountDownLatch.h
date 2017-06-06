#ifndef __COUNTDOWNLATCH_H
#define __COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"


namespace jmuduo{

class CountDownLatch
{
	public:
		explicit CountDownLatch(int count):
			mutex_(),
			cond_(mutex_),
			count_(count)
		{
		}
		
		

		void wait();

		void countdown();

		int getCount() const;
		

	private:
		mutable MutexLock mutex_;
		Condition cond_;
		int count_;
};


}


#endif