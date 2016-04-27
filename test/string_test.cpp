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

static std::string join1(const std::vector<std::string> &vec, const std::string &s_sep)
{
    std::string ret;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (i == 0)
            ret += vec[i];
        else
            ret += s_sep + vec[i];
    }
    return ret;
}

static std::string join2(const std::vector<std::string> &vec, const std::string &s_sep)
{
    std::string ret;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (i == (vec.size() - 1))
            ret += vec[i];
        else
            ret += vec[i] + s_sep;
    }
    return ret;
}

static std::string join3(const std::vector<std::string> &vec, const std::string &s_sep)
{
    std::string ret;
    size_t i = 0;
    for (; i < vec.size(); ++i)
    {
        ret += vec[i] + s_sep;
    }
    ret.erase(ret.find_last_not_of(s_sep) + 1)
    size_t s1 = str.size();
    size_t s2 = suffix.size();
    if ((s1 >= s2) && (str.compare(s1 - s2, s2, suffix) == 0))
    return ret;
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

TEST(StringTest, testJoin)
{
}
