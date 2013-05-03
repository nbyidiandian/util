#include <bits/atomicity.h>
#include <gtest/gtest.h>
#include "scoped_latency.h"

using namespace __gnu_cxx;

TEST(Atomic, AtomicInc)
{
    static const int64_t LOOP_COUNT = 9999999999;
    {
        util::ScopedLatency timer("increment int64_t %d times cost: ");
        int64_t count = 0;
        for (int64_t i = 0; i < LOOP_COUNT; ++i)
        {
            ++count;
        }
    }
    {
        util::ScopedLatency timer("increment atomic_word %d times cost: ");
        _Atomic_word count = 0;
        for (int64_t i = 0; i < LOOP_COUNT; ++i)
        {
            __atomic_add(&count, 1);
        }
    }
    return 0;
}
