#ifndef __TIMERID_H
#define __TIMERID_H

#include <stdint.h>
#include <stddef.h>

namespace jmuduo
{

namespace net
{

class Timer;

class TimerId
{
	public:
		TimerId():
			timer_(NULL),
			sequence_(0)
		{
	
		}
		
		TimerId(Timer* timer,int64_t sequence):
			timer_(timer),
			sequence_(sequence)
		{
		
		}
	
		friend class TimerQueue;

	private:
		Timer* timer_;
		int64_t sequence_;

};

}

}




#endif