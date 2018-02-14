#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

#include "fixed_queue.h"
#include "thread_mutex.h"

static const int MAX_QUEUE_SIZE = 1024*1024*8;
static const int CONSUMER_NUM = 1;
static const int PRODUCER_NUM = 5;

uint64_t getCurrentTime()
{
    struct timeval v;
    if (gettimeofday(&v, NULL) != 0)
    {
        perror("gettimeofday");
        return 0;
    }

    return (v.tv_sec * 1000 * 1000 + v.tv_usec);
}

struct Object
{
    uint64_t seq;
};

typedef FixedQueue<Object> Queue;

ThreadMutex stdout_lock;
Queue queue;

int stop = 0;

void *producer(void *args)
{
    uint64_t producer_id = (uint64_t)args;
    uint64_t seq = 0;
    int ret = 0;
    
    const uint64_t notify_interval = 1024;
    uint64_t now = getCurrentTime();
    uint64_t last = now; 
    
    while (!stop)
    {
        Object *obj = new Object();
        if (obj == NULL)
        {
            continue;
        }
        
        obj->seq = seq;
        
        ret = queue.push(obj);

        if (ret == LF_SIZE_OVERFLOW)
        {
            delete obj;
            continue;
        }

        ++seq;
        
        if (seq % notify_interval == 0)
        {
            now = getCurrentTime();
            
            MutexGuard guard(&stdout_lock);
            fprintf(stdout, "%llu producer sequence=%llu. total=%lld, free=%lld, QPS=%0.2f\n",
                    producer_id, seq, queue.get_total(), queue.get_free(),
                    ((notify_interval*1000*1000.0) / (now-last)));
            last = now;
        }
    }

    {
        MutexGuard guard(&stdout_lock);
        fprintf(stdout, "%llu producer sequence=%llu. total=%lld, free=%lld [END]\n",
                producer_id, seq, queue.get_total(), queue.get_free());
    }

    return NULL;
}

void *consumer(void *args)
{
    uint64_t consumer_id = (uint64_t)args;
    Object *obj = NULL;
    uint64_t seq = 0;
    int ret = 0;
    
    const uint64_t notify_interval = 1024;
    uint64_t now = getCurrentTime();
    uint64_t last = now;
    
    while (!stop)
    {
        ret = queue.pop(obj);
        if (ret == LF_ENTRY_NOT_EXIST)
        {
            continue;
        }
        else if (ret != LF_SUCCESS)
        {
            fprintf(stderr, "pop failed[%d]\n", ret);
            break;
        }

        if (obj == NULL)
        {
            fprintf(stderr, "pop a NULL object\n");
            break;
        }
             
        usleep(1000);
   
        seq = obj->seq;
        delete obj;
        obj = NULL;
        
        if (seq % notify_interval == 0)
        {
            now = getCurrentTime();

            MutexGuard guard(&stdout_lock);
            fprintf(stdout, "%llu consumer sequence=%llu. total=%lld, free=%lld, QPS=%0.2f\n",
                    consumer_id, seq, queue.get_total(), queue.get_free(),
                    ((notify_interval*1000*1000.0) / (now - last)));
        }
    }

    {
        MutexGuard guard(&stdout_lock);
        fprintf(stdout, "%llu consumer sequence=%llu. total=%lld, free=%lld [END]\n",
                consumer_id, seq, queue.get_total(), queue.get_free());
    }

    return NULL;
}

void sig_stop(int num)
{
    stop = 1;
    fprintf(stdout, "quit...\n");
}

int main(int argc, char *argv[])
{
    signal(SIGHUP, sig_stop);
    signal(SIGQUIT, sig_stop);
    signal(SIGINT, sig_stop);
    
    int ret = queue.init(MAX_QUEUE_SIZE);
    if (ret != LF_SUCCESS)
    {
        fprintf(stderr, "init queue error[%d]\n", ret);
        return -1;
    }

    pthread_t producers[PRODUCER_NUM];
    pthread_t consumers[CONSUMER_NUM];

    for (int i = 0; i < PRODUCER_NUM; ++i)
    {
        pthread_create(&producers[i], NULL, producer, (void *)i);
    }

    for (int i = 0; i < CONSUMER_NUM; ++i)
    {
        pthread_create(&consumers[i], NULL, consumer, (void *)i);
    }

    for (int i = 0; i < PRODUCER_NUM; ++i)
    {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < CONSUMER_NUM; ++i)
    {
        pthread_join(consumers[i], NULL);
    }
    
    return 0;
}
