#ifndef __THREADLOCAL_H
#define __THREADLOCAL_H

#include <pthread.h>
#include "Mutex.h"
namespace jmduo
{

template<class T>
class ThreadLocal
{
	public:
		ThreadLocal()
		{
				MCHECK(pthread_key_create(&pkey_,&ThreadLocal::destroy));
		}

		~ThreadLocal()
		{
				MCHECK(pthread_key_delete(pkey_));
		}

		T& value()
		{
				T* obj=static_cast<T*>(pthread_getspecific(pkey_));
				if(!obj)
				{
					T* objx= new T();
					MCHECK(pthread_setspecific(pkey_,static_cast<void*>(objx)));
					obj = objx;
				}
				
				return *obj;
		}

	private:
		
		static void destroy(void* str)
		{
			T* obj = static_cast<T*>(str);
			delete obj;
			
		}

		pthread_key_t pkey_;

};

}



#endif