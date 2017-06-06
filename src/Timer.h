#ifndef __TIMER_H
#define __TIMER_H
#include <boost/function.hpp>

#include "Timestamp.h"
#include "Atomic.h"
#include "Callback.h"
namespace jmuduo
{

namespace net
{
	
class Timer
{

	public:
		
		
		Timer(const TimerCallback& callback,Timestamp when,double interval);

		void run()
		{
			callback_();
		}

		Timestamp expiration() const{return expiration_;}
		int64_t sequence() const{return sequence_;}
		bool repeated() const {return repeated_;}
		
		void restart(Timestamp now);

	private:
		const TimerCallback callback_;
		Timestamp expiration_;
		const bool repeated_;
		const double interval_;
		const int64_t sequence_;

		static AtomicInt64 s_numCreated_;
};
	
	
}


}

#endif