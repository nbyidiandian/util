#ifndef UTIL_THREAD_LOCAL_H
#define UTIL_THREAD_LOCAL_H

#include <pthread.h>

namespace util
{

class Tls
{
public:
	static int pthread_atexit_add(void *arg, void (*free_fn)(void *) );
	static int pthread_atexit_remove(void *arg, void (*free_fn)(void*) );
protected:
	static void pthread_atexit_done(void *arg);
	static void pthread_atexit_init(void);
protected:
	static pthread_key_t 	_pthread_atexit_key;
	static pthread_once_t 	_pthread_atexit_control_once;
};

}

#endif
