#include "gtest/gtest.h"

#include "lcrs/lcrs.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(System, macAddressMap)
{
    EXPECT_GT(System::getMacAddressMap().size(), 0);
}