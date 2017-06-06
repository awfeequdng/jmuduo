#include "Timestamp.h"
#include <sys/time.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include <stdio.h>
using namespace jmuduo;
std::string Timestamp::toString() const{
	char buf[32]={0};
	int64_t seconds=microSecondsSinceEpoch_/kMicroPerSecond;
	int64_t microSeconds = microSecondsSinceEpoch_%kMicroPerSecond;
	snprintf(buf,sizeof(buf),"%" PRId64 ".%6" PRId64 "",seconds,microSeconds);
	return buf;

}

std::string Timestamp::toFormattedString() const{
	char buf[32]={0};
	time_t seconds=static_cast<time_t>(microSecondsSinceEpoch_/kMicroPerSecond);
	struct tm resu;
	gmtime_r(&seconds,&resu);
	int microSeconds = microSecondsSinceEpoch_%kMicroPerSecond;
	snprintf(buf,sizeof(buf),"%4d-%2d-%2d %2d:%2d:%2d.%6d",resu.tm_year+1900,resu.tm_mon+1,resu.tm_mday,resu.tm_hour,resu.tm_min,resu.tm_sec,microSeconds);
	return buf;


}

Timestamp Timestamp::now(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	int64_t seconds=tv.tv_sec;
	return Timestamp(seconds * kMicroPerSecond + tv.tv_usec);

}