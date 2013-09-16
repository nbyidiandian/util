#include <stdint.h>
#include <gtest/gtest.h>
#include "scoped_latency.h"

TEST(CpuCacheTest, testMemoryAccessPerformance)
{
    static const int ARRAY_SIZE = 64 * 1024 * 1024;
    int *arr = new int[ARRAY_SIZE];

    static const int steps[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    for (size_t i = 0; i < (sizeof(steps) / sizeof(int)); ++i)
    {
        int step = steps[i];
        memset(arr, 1, sizeof(int) * ARRAY_SIZE);
        {
            util::ScopedLatency timer("multiplies every %02d-th value in the array by 3 cost: ", step);
            for (int j = 0; j < ARRAY_SIZE; j += step) arr[j] *= 3;
        }
    }
}
