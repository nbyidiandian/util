#ifndef UTIL_SCOPED_LATENCY_H
#define UTIL_SCOPED_LATENCY_H

#include <string>
#include "time_util.h"

namespace util
{

class ScopedLatency
{
public:
    ScopedLatency(const char *format, ...);
    virtual ~ScopedLatency();
private:
    int64_t _start;
    std::string _format;
};

}

#endif // UTIL_SCOPED_LATENCY_H
