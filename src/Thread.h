#ifndef __THREAD_H
#define __THREAD_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>
namespace jmuduo{

typedef boost::function<void()> ThreadFunc;
class Thread
{
	public:
		explicit Thread(const ThreadFunc& threadFunc);

		void start();

		void join();
		bool started()const {return started_;}
		~Thread();
	private:
		bool started_;
		bool joined_;
		ThreadFunc threadFunc_;
		pthread_t pthread_;
		
		boost::shared_ptr<pid_t> tid_;

};





}

#endif