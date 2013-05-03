/**
 * @file   singleton.h
 * @author jianhao@taobao.com
 * @date   2013-01-30
 *
 * @brief singleton
 *
 * Only one instance in process
 */

/*! @addtogroup util
 * @{
 */


#ifndef UTIL_SINGLETON_H
#define UTIL_SINGLETON_H

#include <pthread.h>
#include <stdlib.h> // atexit

namespace util
{

template<typename T>
class Singleton
{
public:
    static T& instance()
    {
        pthread_once(&_init_once, &Singleton::init);
        return *_value;
    }

private:
    Singleton();
    ~Singleton();

    static void init()
    {
        _value = new T();
        ::atexit(destroy);
    }

    static void destroy()
    {
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        delete _value;
    }

private:
    static pthread_once_t _init_once;
    static T*             _value;
};

template<typename T>
pthread_once_t Singleton<T>::_init_once = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::_value = NULL;

}

#endif



/*! @} */
