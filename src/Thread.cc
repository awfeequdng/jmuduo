#include "Thread.h"
#include "CurrentThread.h"

#include <stdio.h>
#include <boost/weak_ptr.hpp>

#include <unistd.h>
#include <sys/syscall.h>

#include <assert.h>
namespace jmuduo{



namespace CurrentThread{
	__thread int t_cachedTid = 0;

	
}

namespace detail{

struct ThreadData{

	boost::weak_ptr<pid_t> tid_;
	ThreadFunc func_;

	ThreadData(const ThreadFunc& func,boost::shared_ptr<pid_t> tid):func_(func),tid_(tid){	}

	void runInThread(){
		//更新tid_
		{
		boost::shared_ptr<pid_t> ti=tid_.lock();
		*ti=static_cast<pid_t>(CurrentThread::tid());
		}
		func_();
	}



};
void* startThread(void *arg){
	ThreadData *threadData=static_cast<ThreadData*>(arg);
	threadData->runInThread();
	return NULL;
}


pid_t gettid(){
	return static_cast<pid_t>(::syscall(SYS_gettid));	

}
}





}


using namespace jmuduo;
bool CurrentThread::isMainThread(){
	return tid()==static_cast<int>(::getpid());

}
void CurrentThread::cacheTid(){
	t_cachedTid=static_cast<int>(detail::gettid());
}

Thread::Thread(const ThreadFunc& threadFunc):
	threadFunc_(threadFunc),
	pthread_(0),
	tid_(new pid_t(0)),
	started_(false),
	joined_(false)
{

}

void Thread::start(){
	assert(started_==false);
	started_=true;
	detail::ThreadData* data=new detail::ThreadData(threadFunc_,tid_);
	if(pthread_create(&pthread_,NULL,&detail::startThread,data)!=0){
		started_=false;
		printf("pthread_create error\n");
	}
}

void Thread::join(){
	assert(started_==true);
	assert(joined_==false);
	joined_=true;
	pthread_join(pthread_,NULL);
}

Thread::~Thread(){
	if(started_==true && joined_==false){
		pthread_detach(pthread_);
	}

}