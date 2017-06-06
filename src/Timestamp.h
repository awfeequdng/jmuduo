#ifndef __TIMESTAMP_H
#define __TIMESTAMP_H

//int64_t need <stdint.h>
#include <stdint.h>
#include <boost/operators.hpp>
#include <string>
#include <time.h>
namespace jmuduo{



class Timestamp:public boost::less_than_comparable<Timestamp>
{
public:

	Timestamp():microSecondsSinceEpoch_(0){}

	explicit Timestamp(int64_t microSecondsSinceEpoch):microSecondsSinceEpoch_(microSecondsSinceEpoch){}

	void swap(Timestamp &that){
		std::swap(microSecondsSinceEpoch_,that.microSecondsSinceEpoch_);
	}

	std::string toString() const;

	std::string toFormattedString() const;

	bool valid() const{return microSecondsSinceEpoch_>0;}

	int64_t microSecondsSinceEpoch() const{ return microSecondsSinceEpoch_;}

	time_t secondsSinceEpoch() const {return microSecondsSinceEpoch_/kMicroPerSecond;}

	static Timestamp now();

	static Timestamp invalid(){
		return Timestamp();
	}

	static Timestamp fromUnixTime(time_t t,int microseconds){
		return Timestamp(t*kMicroPerSecond+microseconds);
	}

	static Timestamp fromUnixTime(time_t t){
		return fromUnixTime(t,0);
	}
	static const int kMicroPerSecond = 1000 * 1000;
private:
	
	int64_t microSecondsSinceEpoch_;

};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}


inline double timeDifference(Timestamp high,Timestamp low)
{
	int64_t diff=high.microSecondsSinceEpoch()-low.microSecondsSinceEpoch();
	return static_cast<double>(diff) / Timestamp::kMicroPerSecond;	
}

inline Timestamp addTime(Timestamp timestamp,double seconds){
	int64_t delta=static_cast<int64_t>(seconds*Timestamp::kMicroPerSecond);
	return Timestamp(timestamp.microSecondsSinceEpoch()+delta);
}

}

#endif