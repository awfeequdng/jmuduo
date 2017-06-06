#ifndef __BOUNDEDBLOCKINGQUEUE_H
#define __BOUNDEDBLOCKINGQUEUE_H

#include "Mutex.h"
#include "Condition.h"

#include <boost/circular_buffer.hpp>

namespace jmuduo
{

template<class T>
class BoundedBlockingQueue
{
	public:
		BoundedBlockingQueue(size_t size):
			mutex_(),
			notFull_(mutex_),
			notEmpty_(mutex_),
			deque_(size)
		{
		}

		void put(T x);

		T take();
		
		bool empty() const ;

		bool full() const;

		size_t size() const;

		size_t capacity() const ;
	private:
		mutable MutexLock mutex_;
		Condition notFull_;
		Condition notEmpty_;
		boost::circular_buffer<T> deque_;
};


}


#endif