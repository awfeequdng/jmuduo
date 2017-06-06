#ifndef __BLOCKINGQUEUE_H
#define __BLOCKINGQUEUE_H
#include "Mutex.h"
#include "Condition.h"

#include <deque>

namespace jmuduo{

template<class T>
class BlockingQueue
{
	public:
		BlockingQueue():
			mutex_(),
			notEmpty_(mutex_),
			deque_()
		{
		}

		~BlockingQueue();

		void put(T x);

		T take();
		
		size_t size() const
		{
			MutexLockGuard lock(mutex_);
			return deque_.size();
		}
	private:
		mutable MutexLock mutex_;
		Condition notEmpty_;
		std::deque<T> deque_;
};

}


#endif