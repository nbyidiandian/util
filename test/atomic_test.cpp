#include <stdint.h>
#include <ext/atomicity.h>
#include <gtest/gtest.h>
#include "scoped_latency.h"

#if __GNUC__ > 4 ||                             \
    (__GNUC__ == 4 && (__GNUC_MINOR__ >= 2))
#include <ext/atomicity.h>
#else
#include <bits/atomicity.h>
#endif

using namespace __gnu_cxx;

TEST(AtomicTest, testAtomicInc)
{
    char msg[1024];
    int64_t count = 0;
    static const int64_t LOOP_COUNT = 999999;
    {
        snprintf(msg, 1024, "increment int64_t %lld times cost: ", LOOP_COUNT);
        util::ScopedLatency timer(msg);
        count = 0;
        for (int64_t i = 0; i < LOOP_COUNT; ++i)
        {
            ++count;
        }
    }
    {
        snprintf(msg, 1024, "increment atomic_word %lld times cost: ", LOOP_COUNT);
        util::ScopedLatency timer(msg);
        _Atomic_word count = 0;
        for (int64_t i = 0; i < LOOP_COUNT; ++i)
        {
            __atomic_add(&count, 1);
        }
    }
}


