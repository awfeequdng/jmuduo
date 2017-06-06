#ifndef __SINGLETON_H
#define __SINGLETON_H

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

namespace jmuduo
{

template<class T>
class Singleton
{
	public:
		static T& instance()
		{
			pthread_once(&ponce_,&init)
			assert(value_ != NULL);
			return *value_;
		}
		
		static void init()
		{
			value_ = new T();
			atexit(&destroy);
		}
		
		static void destroy()
		{
			delete value_;
			value_ = NULL;
		}
	private:
		Singleton(){}
		~Singleton(){}


		static pthread_once_t ponce_;
		static T* value_;

};

template<class T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<class T>
T* Singleton<T>::value_ = NULL;

}

#endif