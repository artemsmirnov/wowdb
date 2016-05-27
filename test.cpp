#include "gtest/gtest.h"

TEST(Store, Test1) {
    EXPECT_EQ(2+2, 4);
}

int main(int ac, char* av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}