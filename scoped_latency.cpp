#include <stdarg.h>
#include "scoped_latency.h"

namespace util
{

ScopedLatency::ScopedLatency(const char *format, ...)
{
    static const int MAX_MESSAGE_LENGTH = 1024;
    char buffer[MAX_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, MAX_MESSAGE_LENGTH, format, args);
    _start = TimeUtil::getCurrentTime();
    va_end(args);
    _format.assign(buffer);
    _format += "%lld\n";
}

ScopedLatency::~ScopedLatency()
{
    int64_t latency = TimeUtil::getCurrentTime() - _start;
    printf(_format.c_str(), latency);
}

}
