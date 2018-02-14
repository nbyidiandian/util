#include <gtest/gtest.h>

namespace util
{
namespace default_constructor_test
{

struct StructUseDefaultConstructor
{
public:
    int32_t m;
};

struct StructWithConstructor
{
public:
    StructWithConstructor() : m(DEFAULT_VALUE) { }
public:
    int32_t m;
private:
    static const int32_t DEFAULT_VALUE = 2;
};

static const char CLEARED_STACK_BYTE_VALUE = 0xF1;
static const int32_t CLEARED_STACK_INT32_VALUE = 0xF1F1F1F1;
static const int64_t CLEARED_STACK_INT64_VALUE = 0xF1F1F1F1F1F1F1F1;

void clearStack() __attribute__ ((noinline))
{
    static const int STACK_SIZE = 1024*1024;
    char array[STACK_SIZE];
    memset(array, CLEARED_STACK_BYTE_VALUE, sizeof(char)*STACK_SIZE);
}

static inline void runFunctionInClearedStack(void (*func)())
{
    clearStack();
    (*func)();
}

void defaultConstructorOfIntTypesTest() __attribute__ ((noinline))
{
    int32_t a;
    int32_t b = int32_t();
    int32_t c;
    int64_t d;
    int64_t e = int64_t();
    int64_t f;

    EXPECT_EQ(a, CLEARED_STACK_INT32_VALUE);
    EXPECT_EQ(b, 0);
    EXPECT_EQ(c, CLEARED_STACK_INT32_VALUE);
    EXPECT_EQ(d, CLEARED_STACK_INT64_VALUE);
    EXPECT_EQ(e, 0);
    EXPECT_EQ(f, CLEARED_STACK_INT64_VALUE);
}

void defaultConstructorOfFloatTypesTest() __attribute__ ((noinline))
{
    float e;
    float f = float();
    float g;
    float h = 0.0f;
}

void defaultConstructorOfStructTest() __attribute__ ((noinline))
{
    StructUseDefaultConstructor a;
    StructUseDefaultConstructor b = StructUseDefaultConstructor();
    StructUseDefaultConstructor c;
    StructWithConstructor d;
    StructWithConstructor e = StructWithConstructor();
    StructWithConstructor f;
    EXPECT_EQ(a.m, CLEARED_STACK_INT32_VALUE);
    EXPECT_EQ(b.m, CLEARED_STACK_INT32_VALUE);
    EXPECT_EQ(c.m, CLEARED_STACK_INT32_VALUE);
    EXPECT_EQ(d.m, StructWithConstructor.DEFAULT_VALUE);
    EXPECT_EQ(e.m, StructWithConstructor.DEFAULT_VALUE);
    EXPECT_EQ(f.m, StructWithConstructor.DEFAULT_VALUE);
}

TEST(ConstructorTest, testDefaultConstructors)
{
    runFunctionInClearedStack(defaultConstructorOfIntTypesTest);
    runFunctionInClearedStack(defaultConstructorOfFloatTypesTest);
    runFunctionInClearedStack(defaultConstructorOfStructTest);
}

} // namespace default_constructor_test
} // namespace util
