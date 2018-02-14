#include <vector>
#include <gtest/gtest.h>

int numberOfCoins(int sum_value, const std::vector<int> &coin_values)
{
    std::vector<int> coin_numbers;
    coin_numbers.resize(sum_value + 1, INT_MAX);
    coin_numbers[0] = 0;
    for (int i = 1; i < sum_value; ++i)
    {
        for (int j = 0; j < coin_values.size(); ++j)
        {
            int coin_value = coin_values[j];
            if (coin_value <= i && coin_numbers[i-coin_value] + 1 < coin_numbers[i])
                coin_numbers[i] = coin_numbers[i-coin_value] + 1;
        }
    }
    return coin_numbers[sum_value];
}

TEST(DynamicProgrammingTest, testCoin)
{
    std::vector<int> coin_values = { 1, 3, 5};
    EXPECT_EQ(0, numberOfCoins(0));
    EXPECT_EQ(1, numberOfCoins(1));
    EXPECT_EQ(2, numberOfCoins(2));
    EXPECT_EQ(1, numberOfCoins(3));
    EXPECT_EQ(2, numberOfCoins(4));
    EXPECT_EQ(1, numberOfCoins(5));
    EXPECT_EQ(2, numberOfCoins(6));
    EXPECT_EQ(3, numberOfCoins(7));
    EXPECT_EQ(2, numberOfCoins(8));
    EXPECT_EQ(3, numberOfCoins(9));
    EXPECT_EQ(2, numberOfCoins(10));
    EXPECT_EQ(3, numberOfCoins(11));
}
