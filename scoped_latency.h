#ifndef UTIL_SCOPED_LATENCY_H
#define UTIL_SCOPED_LATENCY_H

#include <string>
#include "time_util.h"

namespace util
{

class ScopedLatency
{
public:
    ScopedLatency(const std::string &message = "");
    virtual ~ScopedLatency();
private:
    int64_t _start;
    std::string _message;
};

}

#endif // UTIL_SCOPED_LATENCY_H
