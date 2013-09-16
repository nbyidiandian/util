#include <gtest/gtest.h>

struct list_head
{
    struct list_head *next, *prev;
};

TEST(ListTest, testContainerOf)
{
#define container_of(ptr, type, member) ({ \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); })

    struct user_t
    {
        int8_t int8_data;
        list_head *node;
    };
    struct user_t g_user_list;
    struct user_t *address = container_of(&(g_user_list.node), struct user_t, node);
    EXPECT_TRUE(&g_user_list == address);
}

TEST(ListTest, testOffsetOf)
{
    struct user_t_aligned
    {
        int8_t int8_data;
        int16_t int16_data;
        int32_t int32_data;
        int64_t int64_data;
        list_head *node;
    };
    EXPECT_EQ(0ul, offsetof(user_t_aligned, int8_data));
    EXPECT_EQ(2ul, offsetof(user_t_aligned, int16_data));
    EXPECT_EQ(4ul, offsetof(user_t_aligned, int32_data));
    EXPECT_EQ(8ul, offsetof(user_t_aligned, int64_data));

#pragma pack(push, 1)
    struct user_t
    {
        int8_t int8_data;
        int16_t int16_data;
        int32_t int32_data;
        int64_t int64_data;
        list_head *node;
    };
#pragma pack(pop)
    EXPECT_EQ(0ul, offsetof(user_t, int8_data));
    EXPECT_EQ(1ul, offsetof(user_t, int16_data));
    EXPECT_EQ(3ul, offsetof(user_t, int32_data));
    EXPECT_EQ(7ul, offsetof(user_t, int64_data));
}
