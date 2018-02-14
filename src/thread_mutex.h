/**
 * @file   thread_mutex.h
 * @author jianhao@taobao.com
 * @date   2013-01-28
 *
 * @brief thread mutex
 *
 * posix thread lock class
 */

/*! @addtogroup util
 * @{
 */

#ifndef UTIL_THREAD_MUTEX_H
#define UTIL_THREAD_MUTEX_H

#include <pthread.h>
#include <cassert>

#include "time_util.h"

namespace util
{

class SpinLock
{
public:
    //PTHREAD_PROCESS_SHARED
    //PTHREAD_PROCESS_PRIVATE
    SpinLock(int pshared=PTHREAD_PROCESS_PRIVATE)
    {
        pthread_spin_init(&_spinlock, pshared);
    }

    virtual ~SpinLock()
    {
        pthread_spin_destroy(&_spinlock);
    }

    inline int lock()
    {
        return pthread_spin_lock(&_spinlock);
    }

    inline int trylock ()
    {
        return pthread_spin_trylock(&_spinlock);
    }

    inline int unlock()
    {
        return pthread_spin_unlock(&_spinlock);
    }
private:
    SpinLock(const SpinLock&);
    SpinLock& operator = (const SpinLock&);
protected:
    pthread_spinlock_t _spinlock;
};

class ThreadMutex
{
public:
    ThreadMutex(const pthread_mutexattr_t *mta = NULL)
    {
        pthread_mutex_init(&_mutex, mta);
    }
    
    virtual ~ThreadMutex()
    {
        pthread_mutex_destroy(&_mutex);
    }
    
    inline int lock()
    {
        return pthread_mutex_lock(&_mutex);
    }

    inline int trylock ()
    {
        return pthread_mutex_trylock(&_mutex);
    }

    inline int unlock()
    {
        return pthread_mutex_unlock(&_mutex);
    }
private:
    ThreadMutex(const ThreadMutex&);
    ThreadMutex& operator = (const ThreadMutex&);
protected:
    pthread_mutex_t _mutex;
};

class RecursiveThreadMutex : public ThreadMutex
{
public:
    RecursiveThreadMutex()
        : ThreadMutex(RECURSIVE_PTHREAD_MUTEXATTR_PTR)
    {}

private:
    RecursiveThreadMutex(const RecursiveThreadMutex&);
    RecursiveThreadMutex& operator = (const RecursiveThreadMutex&);
private:
    static const pthread_mutexattr_t *RECURSIVE_PTHREAD_MUTEXATTR_PTR;
};

class ThreadCond : public ThreadMutex
{
public:
    ThreadCond()
    {
        pthread_cond_init(&_cond, NULL);
    }

    virtual ~ThreadCond()
    {
        pthread_cond_destroy(&_cond);
    }

    int wait(int64_t usec = 0)
    {
        int ret = 0; 
        if (usec <= 0)
        {
            ret = pthread_cond_wait(&_cond, &_mutex);
        }
        else
        {
            timespec ts = TimeUtil::getTimespec(usec);
            ret = pthread_cond_timedwait(&_cond, &_mutex, &ts);
        }
        
        return ret;
    }
    
    inline int signal()
    {
        return pthread_cond_signal(&_cond);
    }

    inline int broadcast()
    {
        return pthread_cond_broadcast(&_cond);
    }

protected:
    pthread_cond_t  _cond;
};

class ProducerConsumerCond : public ThreadMutex
{
public:
    ProducerConsumerCond()
    {
        pthread_cond_init(&_producerCond, NULL);
        pthread_cond_init(&_consumerCond, NULL);
    }

    ~ProducerConsumerCond()
    {
        pthread_cond_destroy(&_producerCond);
        pthread_cond_destroy(&_consumerCond);
    }
public:
    inline int producerWait(int64_t usec = 0)
    {
        return wait(_producerCond, usec);
    }

    inline int signalProducer()
    {
        return signal(_producerCond);
    }

    inline int broadcastProducer()
    {
        return broadcast(_producerCond);
    }

    inline int consumerWait(int64_t usec = 0)
    {
        return wait(_consumerCond, usec);
    }

    inline int signalConsumer()
    {
        return signal(_consumerCond);
    }

    inline int broadcastConsumer()
    {
        return broadcast(_consumerCond);
    }

private:
    int wait(pthread_cond_t &cond, int64_t usec)
    {
        int ret = 0; 
        if (usec <= 0)
        {
            ret = pthread_cond_wait(&cond, &_mutex);
        }
        else
        {
            timespec ts = TimeUtil::getTimespec(usec);
            ret = pthread_cond_timedwait(&cond, &_mutex, &ts);
        }
        
        return ret;
    }
    
    inline static int signal(pthread_cond_t &cond)
    {
        return pthread_cond_signal(&cond);
    }
    
    inline static int broadcast(pthread_cond_t &cond)
    {
        return pthread_cond_broadcast(&cond);
    }

protected:
    pthread_cond_t  _producerCond;
    pthread_cond_t  _consumerCond;
};

class ScopedLock
{
private:
    ThreadMutex &_lock;
private:
    ScopedLock(const ScopedLock &);
    ScopedLock& operator= (const ScopedLock &);
    
public:
    explicit ScopedLock(ThreadMutex& lock) : _lock(lock)
    {
        int ret = _lock.lock();
        assert(ret == 0); (void) ret;
    }

    ~ScopedLock()
    {
        int ret = _lock.unlock();
        assert(ret == 0); (void) ret;
    }
};

class ReadWriteLock
{
private:
    ReadWriteLock(const ReadWriteLock&);
    ReadWriteLock& operator = (const ReadWriteLock&);
public:
    enum Mode
    {
        E_PREFER_READER,
        E_PREFER_WRITER
    };

    ReadWriteLock(Mode mode = E_PREFER_WRITER)
    {
        pthread_rwlockattr_t attr;
        pthread_rwlockattr_init(&attr);
        switch (mode)
        {
        case E_PREFER_WRITER:
            pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
            break;
        case E_PREFER_READER:
            pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_READER_NP);
            break;
        default:
            break;
        }
        pthread_rwlock_init(&_lock, &attr);
    }

    virtual  ~ReadWriteLock()
    {
        pthread_rwlock_destroy(&_lock);
    }

    inline int rdlock()
    {
        return pthread_rwlock_rdlock(&_lock);
    }

    inline int wrlock()
    {
        return pthread_rwlock_wrlock(&_lock);
    }

    inline int tryrdlock()
    {
        return pthread_rwlock_tryrdlock(&_lock);
    }

    inline int trywrlock()
    {
        return pthread_rwlock_trywrlock(&_lock);
    }

    inline int unlock()
    {
        return pthread_rwlock_unlock(&_lock);
    }

protected:
    pthread_rwlock_t _lock;
};

class ScopedReadLock
{
private:
    ReadWriteLock &_lock;
private:
    ScopedReadLock(const ScopedReadLock&);
    ScopedReadLock& operator = (const ScopedReadLock&);
public:
    explicit ScopedReadLock(ReadWriteLock &lock)
        : _lock(lock) 
    {
        int ret = _lock.rdlock();
        assert(ret == 0);  (void) ret;
    }
    ~ScopedReadLock()
    {
        int ret = _lock.unlock();
        assert(ret == 0); (void) ret;
    }
};

class ScopedWriteLock
{
private:
    ReadWriteLock &_lock;
private:
    ScopedWriteLock(const ScopedWriteLock&);
    ScopedWriteLock& operator = (const ScopedWriteLock&);
public:
    explicit ScopedWriteLock(ReadWriteLock &lock)
        : _lock(lock) 
    {
        int ret = _lock.wrlock();
        assert(ret == 0); (void) ret;
    }
    ~ScopedWriteLock() {
        int ret = _lock.unlock();
        assert(ret == 0); (void) ret;
    }
};

class ScopedReadWriteLock
{
private:
    ReadWriteLock& _lock;
    char _mode;
private:
    ScopedReadWriteLock(const ScopedReadWriteLock&);
    ScopedReadWriteLock& operator = (const ScopedReadWriteLock&);

public:
    explicit ScopedReadWriteLock(ReadWriteLock& lock, const char mode)
        : _lock(lock), _mode(mode)
    {
        if (_mode == 'r' || _mode == 'R')
        {
            int ret = _lock.rdlock();
            assert(ret == 0); (void) ret;
        }
        else if (_mode == 'w' || _mode == 'W')
        {
            int ret = _lock.wrlock();
            assert(ret == 0); (void) ret;
        }
    }
    
    ~ScopedReadWriteLock()
    {
        if (_mode == 'r' || _mode == 'R' 
            || _mode == 'w' || _mode == 'W') 
        { 
            int ret = _lock.unlock();
            assert(ret == 0); (void) ret;
        }
    }
};

class Notifier
{
public:
    static const int EXITED = 1 << 16 + 1;
public:
    Notifier() : _accumulatedNotification(0), _exitFlag(false)
    {}
    
    int notifyExit()
    {
        ScopedLock lock(_cond);
        _exitFlag = true;
        return _cond.broadcast();
    }
    
    int notify()
    {
        ScopedLock lock(_cond);
        ++_accumulatedNotification;
        return _cond.signal();
    }
    
    /**
     * return 0 if got notified, and
     * return errno or EXITED
     */
    int waitNotification(int timeout = -1)
    {
        return wait(timeout);
    }

private:
    volatile int _accumulatedNotification;
    volatile bool _exitFlag;
    ThreadCond _cond;

private:    
    int wait(int timeout)
    {
        ScopedLock lock(_cond);
        while (true)
        {
            if (_exitFlag)
            {
                return EXITED;
            }

            if (_accumulatedNotification > 0)
            {
                _accumulatedNotification--;
                return 0;
            } 
            
            int ret = _cond.wait(timeout);
            if (ret != 0)
            {
                return ret;
            }
        }
    }
};

}

#endif //


/*! @} */
