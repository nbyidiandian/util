#include <string>
#include <gtest/gtest.h>
#include "scoped_latency.h"

static std::string appendCharToString(size_t len, char c)
{
    std::string str;
    str.reserve(len);
    for (size_t i = 0; i < len; ++i)
    {
        str.push_back(c);
    }
    return str;
}

static std::string appendCharToStringWithoutCheck(size_t len, char c)
{
    std::string str;
    str.reserve(len);
    char *buffer = &(str[0]);
    for (size_t i = 0; i < len; ++i)
    {
        buffer[i] = c;
    }
    return str;
}

static std::string appendCharToArray(size_t len, char c)
{
    char *buffer = new char[len + 1];
    for (size_t i = 0; i < len; ++i)
    {
        buffer[i] = c;
    }
    std::string str(buffer, len);
    delete buffer;
    return str;
}

TEST(StringTest, testPushBack)
{
    static const size_t LOOP_COUNT = 10000;
    static const size_t STRING_LENGTH = 1024;
    {
        util::ScopedLatency timer("push_back characters to string cost: ");
        for (size_t i = 0; i < LOOP_COUNT; ++i)
        {
            appendCharToString(STRING_LENGTH, 'a');
        }
    }
    {
        util::ScopedLatency timer("push_back characters to string without check cost: ");
        for (size_t i = 0; i < LOOP_COUNT; ++i)
        {
            appendCharToStringWithoutCheck(STRING_LENGTH, 'a');
        }
    }
    {
        util::ScopedLatency timer("add characters to array cost: ");
        for (size_t i = 0; i < LOOP_COUNT; ++i)
        {
            appendCharToArray(STRING_LENGTH, 'a');
        }
    }
}
