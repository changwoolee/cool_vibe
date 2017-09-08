#ifndef __MY_THREAD_H__
#define __MY_THREAD_H__

#include <pthread.h>
class MyThreadClass
{
public:
	MyThreadClass() {/* empty */}
  	virtual ~MyThreadClass() {/* empty */}

	bool StartInternalThread(){
		return (pthread_create(&_thread, NULL, InternalThreadEntryFunc, this)==0);
	}

  	void WaitForInternalThreadToExit(){
		(void) pthread_join(_thread,NULL);
	}

protected:
/* Implement this method in your subclass with the code you want your thread to run. */
	virtual void InternalThreadEntry() = 0;

private:
        static void * InternalThreadEntryFunc(void * This){
		((MyThreadClass *)This)->InternalThreadEntry();
		return NULL;
	}
	pthread_t _thread;
};
#endif /* my_thread.h */
