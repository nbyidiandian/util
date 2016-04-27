#include <memory>
#include "thread.h"

using namespace std;

namespace util
{

struct ThreadFunctionWrapperArg
{
    std::tr1::function<void ()> threadFunction;
};

void* Thread::threadWrapperFunction(void* arg)
{
    auto_ptr<ThreadFunctionWrapperArg> p(static_cast<ThreadFunctionWrapperArg*>(arg));
    p->threadFunction();
    return NULL;
}

ThreadPtr Thread::createThread(const std::tr1::function<void ()>& threadFunction)
{
    ThreadPtr threadPtr(new Thread);
    ThreadFunctionWrapperArg* arg = new ThreadFunctionWrapperArg;
    arg->threadFunction = threadFunction;
    
    int rtn = pthread_create(&threadPtr->_id, NULL,
                             &Thread::threadWrapperFunction, arg);
    
    if (rtn != 0) {
        delete arg;
        threadPtr->_id = 0;
        threadPtr.reset();
    }
    
    return threadPtr;
}

}
