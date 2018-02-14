#include <gtest/gtest.h>

struct list_head
{
    struct list_head *next, *prev;
};

TEST(ListTest, testContainerOf)
{
#define container_of(ptr, type, member) ({ \
    const decltype(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); })

    struct user_t
    {
        int8_t int8_data;
        list_head *node;
    };
    struct user_t g_user_list;
    struct user_t *address = container_of(&(g_user_list.node), struct user_t, node);
    ASSERT_EQ(address, &g_user_list);
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
    ASSERT_EQ(0ul, offsetof(user_t_aligned, int8_data));
    ASSERT_EQ(2ul, offsetof(user_t_aligned, int16_data));
    ASSERT_EQ(4ul, offsetof(user_t_aligned, int32_data));
    ASSERT_EQ(8ul, offsetof(user_t_aligned, int64_data));

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
    ASSERT_EQ(0ul, offsetof(user_t, int8_data));
    ASSERT_EQ(1ul, offsetof(user_t, int16_data));
    ASSERT_EQ(3ul, offsetof(user_t, int32_data));
    ASSERT_EQ(7ul, offsetof(user_t, int64_data));
}
