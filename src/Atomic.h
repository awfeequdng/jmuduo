#ifndef __ATOMIC_H
#define __ATOMIC_H


#include <stdint.h>
#include <boost/noncopyable.hpp>

namespace jmuduo{


namespace detail{
template<class T>
class Atomic
{
		private:
			volatile T val_;
		public:
			Atomic():val_(0){}
			Atomic(T val):val_(val){}
			T get(){
				return __sync_val_compare_and_swap(&val_,0,0);
			}
			T getAndAdd(T x){
				return __sync_fetch_and_add(&val_,x);
			}
			T addAndGet(T x){
				return __sync_add_and_fetch(&val_,x);
			}
			T incrementAndGet(){
				return addAndGet(1);
			}
			T decrementAndGet(){
				return addAndGet(-1);
			}
			void add(T x){
				getAndAdd(x);
			}
			void increment(){
				incrementAndGet();
			}
			void decrement(){
				decrementAndGet();
			}
			T getAndSet(T newval){
				return __sync_lock_test_and_set(&val_,newval);
			}
};

}
	typedef detail::Atomic<int32_t> AtomicInt32;
	typedef detail::Atomic<int64_t> AtomicInt64;


}

#endif