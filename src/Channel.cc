#include "Channel.h"
#include "EventLoop.h"

#include <poll.h>
#include <stdio.h>
#include <assert.h>
using namespace jmuduo;
using namespace jmuduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN|POLLPRI;
const int Channel::kWriteEvent = POLLOUT;


Channel::Channel(EventLoop* loop,int fd):
	loop_(loop),
	eventHandling_(false),
	tied_(false),
	fd_(fd),
	events_(0),
	revents_(0),
	index_(-1)
	
{
	
}
Channel::~Channel()
{
	assert(!eventHandling_);

}
bool Channel::isNoneEvent() const
{
	return events_ == kNoneEvent;
}


void Channel::tie(const boost::shared_ptr<void>& obj)
{
	tie_ = obj;
	tied_ = true;
}
void Channel::disableAll()
{
	events_ = kNoneEvent;
	update();
}

void Channel::remove()
{
	assert(isNoneEvent());
	loop_->removeChannel(this);

}
void Channel::update()
{
	loop_->updateChannel(this);
}
void Channel::handleEvent(Timestamp receiveTime)
{
	boost::shared_ptr<void> guard;
	if(tied_)
	{	
		guard = tie_.lock();
		if(guard)
		{
			handleEventWithGuard(receiveTime);
		}
	}
	else
	{
		handleEventWithGuard(receiveTime);
	}
}


void Channel::handleEventWithGuard(Timestamp receiveTime)
{
	eventHandling_ = true;
	if( (revents_ & POLLHUP) && !(revents_&POLLIN))
	{
		if(closeCallback_)
			closeCallback_();
	}
	if(revents_ & POLLNVAL)
	{
		printf("handleEvents() POLLNVAL fd not open\n");
	}
	if(revents_ & (POLLERR | POLLNVAL))
	{
		if(errorCallback_)
			errorCallback_();
	}

	if(revents_ & (POLLIN | POLLPRI | POLLHUP))
	{
		if(readCallback_)
			readCallback_(receiveTime);
	}

	if(revents_ & POLLOUT)
	{
		if(writeCallback_)
			writeCallback_();
	}
	eventHandling_ = false;
}