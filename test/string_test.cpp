#include <string>
#include <cstddef>
#include <gtest/gtest.h>
#include "scoped_latency.h"

class StringTest : public ::testing::Test
{
public:
    StringTest();
    virtual ~StringTest();
public:
    void SetUp();
    void TearDown();
protected:
    std::string appendCharToString(size_t, char);
    std::string appendCharToStringWithoutCheck(size_t len, char c);
    std::string appendCharToArray(size_t len, char c);
    std::string join1(const std::vector<std::string> &vec, const std::string &s_sep);
    std::string join2(const std::vector<std::string> &vec, const std::string &s_sep);
};

StringTest::StringTest() {
}

StringTest::~StringTest() {
}

void StringTest::SetUp() {
}

void StringTest::TearDown() {
}

std::string StringTest::appendCharToString(size_t len, char c)
{
    std::string str;
    str.reserve(len);
    for (size_t i = 0; i < len; ++i)
    {
        str.push_back(c);
    }
    return str;
}

std::string StringTest::appendCharToStringWithoutCheck(size_t len, char c)
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

std::string StringTest::appendCharToArray(size_t len, char c)
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

std::string StringTest::join1(const std::vector<std::string> &vec, const std::string &s_sep)
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

std::string StringTest::join2(const std::vector<std::string> &vec, const std::string &s_sep)
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

TEST_F(StringTest, testPushBack)
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

TEST_F(StringTest, testJoin)
{
}
