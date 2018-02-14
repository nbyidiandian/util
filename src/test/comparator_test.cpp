#include <string>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include "scoped_latency.h"

bool Less(const std::string &a, const std::string &b)
{
    return (b < a);
}
bool GreatOrEqual(const std::string &a, const std::string &b)
{
    return (a >= b);
}

TEST(ComparatorTest, testLess)
{
    std::string array[] = { "1", "2", "8", "5", "4", "8", "7", "6" };
    std::string sorted_array[] = { "8", "8", "7", "6", "5", "4", "2", "1" };
    
    std::vector<std::string> expect_sorted_vec(sorted_array,
                                               sorted_array + sizeof(sorted_array)/sizeof(std::string));

    std::vector<std::string> copy1(array, array + sizeof(array)/sizeof(std::string));
    std::sort(copy1.begin(), copy1.end(), Less);
    EXPECT_EQ(copy1, expect_sorted_vec);
    
    std::vector<std::string> copy2(array, array + sizeof(array)/sizeof(std::string));
    std::sort(copy2.begin(), copy2.end(), GreatOrEqual);
}
