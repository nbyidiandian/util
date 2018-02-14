#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <gtest/gtest.h>
#include "scoped_latency.h"

bool compare(double a, double b)
{
    return a < b;
}

TEST(SortTest, testSortNanArray)
{
    // any number compare with NaN will return true
    std::vector<double> array = { 0.0f, 3.0f, NAN, 2.0f, 1.0f };
    std::sort(array.begin(), array.end(), compare);
    ASSERT_DOUBLE_EQ(0.0f, array[0]);
    EXPECT_NE(1.0f, array[1]);
    EXPECT_NE(NAN, array[2]);
    EXPECT_NE(NAN, NAN);
}
