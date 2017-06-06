#ifndef __CHANNEL_H
#define __CHANNEL_H

#include <boost/function.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "Timestamp.h"
namespace jmuduo
{

namespace net
{
class EventLoop;

class  Channel
{
	public:
		typedef boost::function<void()> EventCallback;
		typedef boost::function<void(Timestamp)> ReadEventCallback;

		Channel(EventLoop* loop,int fd);

		~Channel();

		void setReadCallback(const ReadEventCallback& readCallback){ readCallback_ = readCallback;}

		void setWriteCallback(const EventCallback& writeCallback){ writeCallback_ = writeCallback;}

		void setErrorCallback(const EventCallback& errorCallback){ errorCallback_ = errorCallback;}

		void setCloseCallback(const EventCallback& closeCallback){ closeCallback_ = closeCallback;}

		int fd() const {return fd_;}
		void set_index(int index) { index_ = index;}
		void set_revents(int revents) { revents_ = revents;}
		int index() const {return index_;}
		int events() const {return events_;}
		int revents() const {return revents_;}
		EventLoop* ownerLoop(){return loop_;}

		bool isWriting() const {return events_ & kWriteEvent;}
		bool isNoneEvent() const;

		void enableReading() {events_ |= kReadEvent;update();}	
		void enableWriting() {events_ |= kWriteEvent;update();}

		void disableWriting() {events_ &= ~kWriteEvent;update();}
		void disableAll();
		void remove();
		void handleEvent(Timestamp receiveTime);
		void tie(const boost::shared_ptr<void>& obj);

	private:
		void update();
		void handleEventWithGuard(Timestamp receiveTime);
		static const int kNoneEvent;
		static const int kReadEvent;
		static const int kWriteEvent;
		EventLoop* loop_;
		bool eventHandling_;
		boost::weak_ptr<void> tie_;
		bool tied_;
		const int fd_;
		int index_;
		int events_;
		int revents_;

		

		ReadEventCallback readCallback_;
		EventCallback writeCallback_;
		EventCallback errorCallback_;
		EventCallback closeCallback_;

};


}

}


#endif