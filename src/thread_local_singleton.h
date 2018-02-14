/**
 * @file   thread_local_singleton.h
 * @author jianhao@taobao.com
 * @date   2013-01-30
 *
 * @brief thread local singleton
 *
 * Singleton interface, each thread has an instance
 */

/*! @addtogroup util
 * @{
 */


#ifndef UTIL_THREAD_LOCAL_SINGLETON_H
#define UTIL_THREAD_LOCAL_SINGLETON_H

#include <assert.h>
#include <pthread.h>

namespace util
{

template<typename T>
class ThreadLocalSingleton : noncopyable
{
public:
    static T& instance()
    {
        if (_value == NULL)
        {
            _value = new T();
            _deleter.set(_value);
        }
        return *_value;
    }

    static T* pointer()
    {
        return _value;
    }

private:
    static void destructor(void* obj)
    {
        assert(obj == _value);
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        delete _value;
        _value = NULL;
    }

    class Deleter
    {
    public:
        Deleter()
        {
            pthread_key_create(&_key, &ThreadLocalSingleton::destructor);
        }

        ~Deleter()
        {
            pthread_key_delete(_key);
        }

        void set(T* obj)
        {
            assert(pthread_getspecific(_key) == NULL);
            pthread_setspecific(_key, obj);
        }

        pthread_key_t _key;
    };

    static __thread T* _value;
    static Deleter _deleter;
};

template<typename T>
__thread T* ThreadLocalSingleton<T>::_value = 0;

template<typename T>
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::_deleter;

}

#endif


/*! @} */
