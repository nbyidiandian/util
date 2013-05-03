#include "scoped_latency.h"

namespace util
{

ScopedLatency::ScopedLatency(const std::string &message)
        : _message(message)
{
    _start = TimeUtil::getCurrentTime();
}

ScopedLatency::~ScopedLatency()
{
    int64_t latency = TimeUtil::getCurrentTime() - _start;
    fprintf(stdout, "%s%ld\n", _message.c_str(), latency);
}

}
