#include <stdint.h>
#include <stdio.h>
#include <sstream>
#include <gtest/gtest.h>
#include "scoped_latency.h"

struct ThreadArgs
{
    int32_t thread_count;
    int64_t input;
    int64_t loop_count;
};

std::string snprintfItoa(int64_t i)
{
    char buf[64];
    int len = snprintf(buf, 64, "%ld", i);
    return std::string(buf, len);
}

std::string streamItoa(int64_t i)
{
    std::ostringstream ss;
    ss << i;
    return ss.str();
}

void *snprintfRoutine(void *p_args)
{
    const ThreadArgs *args = static_cast<const ThreadArgs *>(p_args);
    for (int i = 0; i < args->loop_count; ++i)
    {
        snprintfItoa(args->input);
    }
    return NULL;
}

void *streamRoutine(void *p_args)
{
    const ThreadArgs *args = static_cast<const ThreadArgs *>(p_args);
    for (int i = 0; i < args->loop_count; ++i)
    {
        streamItoa(args->input);
    }
    return NULL;
}

void testMain(const ThreadArgs &args,
              const char *name,
              void *(routine)(void *))
{
    pthread_t threads[args.thread_count];
    util::ScopedLatency timer("%d threads number to string(%s) "
                              "%lld times cost: ",
                              args.thread_count,
                              name,
                              args.loop_count);
    int ret = 0;
    for (int i = 0; i < args.thread_count; ++i)
    {
        ret = pthread_create(threads + i, NULL, routine, (void *)&args);
        EXPECT_EQ(0, ret);
    }
    void *join_value;
    for (int i = 0; i < args.thread_count; ++i)
    {
        ret = pthread_join(threads[i], &join_value);
        EXPECT_EQ(0, ret);
    }
}

TEST(StreamTest, testStringStream)
{
    ThreadArgs args;
    args.input = 10000;
    args.loop_count = 10000;
    args.thread_count = 2;
    testMain(args, "snprinf", snprintfRoutine);
    testMain(args, "stringstream", streamRoutine);
}
