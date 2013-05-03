#include <list>
#include "sp_basic/common/common.h"
#include "Log.h"
#include "Tls.h"

using namespace std;

namespace sp_basic
{
#define	TLS_OUT_OF_INDEXES          0xffffffff

typedef struct pthread_atexit
{
	void   (*free_fn)(void *);
	void   *arg;
}pthread_atexit_t;

typedef std::list<pthread_atexit_t *> TlsList;

pthread_key_t 	Tls::_pthread_atexit_key = TLS_OUT_OF_INDEXES;
pthread_once_t 	Tls::_pthread_atexit_control_once = PTHREAD_ONCE_INIT;

void Tls::pthread_atexit_done(void *arg)
{
	TlsList *id_list = (TlsList*) arg;
	pthread_atexit_t *id_ptr=NULL;
	TLOG("invoke Tls::pthread_atexit_done(): tid=%ld",gettid());
	for(TlsList::iterator iter=id_list->begin(); iter !=id_list->end(); ++iter)
	{
		id_ptr = *iter;
		if (id_ptr == NULL)
			continue;
		if (id_ptr->free_fn)
			id_ptr->free_fn(id_ptr->arg);
		delete id_ptr;
	}
	delete id_list;
}

void Tls::pthread_atexit_init(void)
{
	// 生成进程空间内所有线程的线程局部变量所使用的键值
	pthread_key_create(&_pthread_atexit_key, pthread_atexit_done);
}

int Tls::pthread_atexit_add(void *arg, void (*free_fn)(void *))
{
	const char *myname = "acl_pthread_atexit_add";
	pthread_atexit_t *id;
	TlsList *id_list;

	if (arg == NULL)
	{
		return 0;
	}
	pthread_once(&_pthread_atexit_control_once, pthread_atexit_init);
	if (_pthread_atexit_key == (pthread_key_t) TLS_OUT_OF_INDEXES)
	{
		TWARN("%s(%d): _pthread_atexit_key(%d) invalid",
				myname, __LINE__, _pthread_atexit_key);
		return (-1);
	}

	id = new pthread_atexit_t;
	if (id == NULL)
	{
		TWARN("%s(%d): new pthread_atexit_t error", myname, __LINE__);
		return -1;
	}
	id->free_fn = free_fn;
	id->arg = arg;

	id_list = (TlsList*) pthread_getspecific(_pthread_atexit_key);
	if (id_list == NULL)
	{
		id_list = new TlsList();
		if (pthread_setspecific(_pthread_atexit_key, id_list) != 0)
		{
			TWARN("%s(%d): pthread_setspecific error, key(%d)",
					myname, __LINE__, _pthread_atexit_key);
			return -1;
		}
	}
	id_list->push_back(id);
	return 0;
}

int Tls::pthread_atexit_remove(void *arg, void (*free_fn)(void*))
{
	const char *myname = "acl_pthread_atexit_remove";
	TlsList *id_list;

	if (arg == NULL)
	{
		return (-1);
	}
	if (_pthread_atexit_key == (pthread_key_t) TLS_OUT_OF_INDEXES)
	{
		TWARN("%s(%d): _pthread_atexit_key(%d)  invalid",myname, __LINE__, _pthread_atexit_key);
		return (-1);
	}
	id_list = (TlsList*) pthread_getspecific(_pthread_atexit_key);
	if (id_list == NULL)
	{
		TWARN("%s(%d): _pthread_atexit_key(%d) no exist in tid(%lu)",
			myname, __LINE__, _pthread_atexit_key,(unsigned long) pthread_self());
		return (-1);
	}
	pthread_atexit_t *id_ptr =NULL;
	TlsList::iterator iter=id_list->begin();
	for(; iter !=id_list->end(); ++iter)
	{
		id_ptr = *iter;
		if (id_ptr == NULL)
			continue;
		if (id_ptr->free_fn == free_fn && id_ptr->arg == arg)
		{
			break;
		}
	}
	if(id_ptr != NULL)
	{
		id_list->erase(iter);
		delete id_ptr;
	}
	return (0);
}



}
