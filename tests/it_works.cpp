#include <gtest/gtest.h>
#include <advanced_math.h>

TEST(ItWorksTest, Addition) {
  EXPECT_EQ(arch::two_plus_two(), 4);
}
